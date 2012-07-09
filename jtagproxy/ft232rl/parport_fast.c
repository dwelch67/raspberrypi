/***************************************************************************
 *   Copyright (C) 2005 by Dominic Rath                                    *
 *   Dominic.Rath@gmx.de                                                   *
 *                                                                         *
 *   Copyright (C) 2008 by Spencer Oliver                                  *
 *   spen@spen-soft.co.uk                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <jtag/interface.h>
#include "bitbang.h"

#include <ftdi.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <sys/ioctl.h>
#include <time.h>

static struct ftdi_context ftdi;
#define BBDATALEN 256
static unsigned char bbdata[BBDATALEN];
static unsigned int bbhead;


#define BIT_TXD_TDI     0
#define BIT_DTR_TMS     4
#define BIT_RTS_TCK     2

#define BIT_RXD_TDO     1
#define BIT_RI__TRST    7
#define BIT_DSR_SRST    5


/* parallel port cable description
 */
struct cable {
    char *name;
    uint8_t TDO_MASK;   /* status port bit containing current TDO value */
    uint8_t TRST_MASK;  /* data port bit for TRST */
    uint8_t TMS_MASK;   /* data port bit for TMS */
    uint8_t TCK_MASK;   /* data port bit for TCK */
    uint8_t TDI_MASK;   /* data port bit for TDI */
    uint8_t SRST_MASK;  /* data port bit for SRST */
    uint8_t OUTPUT_INVERT;  /* data port bits that should be inverted */
    uint8_t INPUT_INVERT;   /* status port that should be inverted */
    uint8_t PORT_INIT;  /* initialize data port with this value */
    uint8_t PORT_EXIT;  /* de-initialize data port with this value */
    uint8_t LED_MASK;   /* data port bit for LED */
};

static struct cable cables[] = {
    /* name                 tdo   trst  tms   tck   tdi   srst  o_inv i_inv init  exit  led */
    //{ "jtagproxy",          0x10, 0x01, 0x04, 0x08, 0x02, 0x20, 0x00, 0x00, 0x21, 0x21, 0x00 },
    {
        "jtagproxyftdi",    //name
        1<<BIT_RXD_TDO,     //tdo
        1<<BIT_RI__TRST,    //trst
        1<<BIT_DTR_TMS,     //tms
        1<<BIT_RTS_TCK,     //tck
        1<<BIT_TXD_TDI,     //tdi
        1<<BIT_DSR_SRST,    //srst
        0x00,               //o_inv
        0x00,               //i_inv
        (1<<BIT_RI__TRST)|(1<<BIT_DSR_SRST),    //init
        (1<<BIT_RI__TRST)|(1<<BIT_DSR_SRST),    //exit
        0x00                //led
    },
    { "wiggler",            0x80, 0x10, 0x02, 0x04, 0x08, 0x01, 0x01, 0x80, 0x80, 0x80, 0x00 },
    { "wiggler2",           0x80, 0x10, 0x02, 0x04, 0x08, 0x01, 0x01, 0x80, 0x80, 0x00, 0x20 },
    { "wiggler_ntrst_inverted", 0x80, 0x10, 0x02, 0x04, 0x08, 0x01, 0x11, 0x80, 0x80, 0x80, 0x00 },
    { "old_amt_wiggler",        0x80, 0x01, 0x02, 0x04, 0x08, 0x10, 0x11, 0x80, 0x80, 0x80, 0x00 },
    { "arm-jtag",           0x80, 0x01, 0x02, 0x04, 0x08, 0x10, 0x01, 0x80, 0x80, 0x80, 0x00 },
    { "chameleon",          0x80, 0x00, 0x04, 0x01, 0x02, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00 },
    { "dlc5",               0x10, 0x00, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00 },
    { "triton",             0x80, 0x08, 0x04, 0x01, 0x02, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00 },
    { "lattice",            0x40, 0x10, 0x04, 0x02, 0x01, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00 },
    { "flashlink",          0x20, 0x10, 0x02, 0x01, 0x04, 0x20, 0x30, 0x20, 0x00, 0x00, 0x00 },
/* Altium Universal JTAG cable. Set the cable to Xilinx Mode and wire to target as follows:
    HARD TCK - Target TCK
    HARD TMS - Target TMS
    HARD TDI - Target TDI
    HARD TDO - Target TDO
    SOFT TCK - Target TRST
    SOFT TDI - Target SRST
*/
    { "altium",         0x10, 0x20, 0x04, 0x02, 0x01, 0x80, 0x00, 0x00, 0x10, 0x00, 0x08 },
    { NULL,             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};


/* configuration */
static char *parport_cable;
static uint16_t parport_port;
static bool parport_exit;
//static uint32_t parport_toggling_time_ns = 1000;
//static int wait_states;

/* interface variables
 */
static struct cable *cable;
static uint8_t dataport_value;

static int ftdi_bbdump ( void )
{
    if(bbhead)
    {
        if (ftdi_write_data(&ftdi,bbdata,bbhead) < 0)
        {
            LOG_ERROR("ftdi_write_data failed");
            return(1);
        }
        bbhead=0;
    }
    return(0);
}

static int ftdi_bbadd ( unsigned char data )
{
    if(bbhead==BBDATALEN)
    {
        if(ftdi_bbdump()) return(1);
    }
    bbdata[bbhead++]=data;
    return(0);
}

static int parport_read(void)
{
    int data;
    unsigned char x;
    int ret;

    ftdi_bbdump();

    x=0;
    if(ftdi_read_pins(&ftdi,&x)<0)
    {
        LOG_ERROR("ftdi_read_pins failed");
    }
    data=x;
    if ((data ^ cable->INPUT_INVERT) & cable->TDO_MASK)
        ret=1;
    else
        ret=0;

    //printf("%u",ret); fflush(stdout);
    return(ret);
}

static inline void parport_write_data(void)
{
    uint8_t output;
    output = dataport_value ^ cable->OUTPUT_INVERT;

    ftdi_bbadd(output);
}

static void parport_write(int tck, int tms, int tdi)
{
    //int i = wait_states + 1;

    if (tck)
        dataport_value |= cable->TCK_MASK;
    else
        dataport_value &= ~cable->TCK_MASK;

    if (tms)
        dataport_value |= cable->TMS_MASK;
    else
        dataport_value &= ~cable->TMS_MASK;

    if (tdi)
        dataport_value |= cable->TDI_MASK;
    else
        dataport_value &= ~cable->TDI_MASK;

    //while (i-- > 0)
        parport_write_data();
}

/* (1) assert or (0) deassert reset lines */
static void parport_reset(int trst, int srst)
{
    LOG_DEBUG("trst: %i, srst: %i", trst, srst);

    if (trst == 0)
        dataport_value |= cable->TRST_MASK;
    else if (trst == 1)
        dataport_value &= ~cable->TRST_MASK;

    if (srst == 0)
        dataport_value |= cable->SRST_MASK;
    else if (srst == 1)
        dataport_value &= ~cable->SRST_MASK;

    parport_write_data();
    ftdi_bbdump();
}

/* turn LED on parport adapter on (1) or off (0) */
static void parport_led(int on)
{
    if (on)
        dataport_value |= cable->LED_MASK;
    else
        dataport_value &= ~cable->LED_MASK;

    parport_write_data();
}

static int parport_speed(int speed)
{
    //wait_states = speed;
    return ERROR_OK;
}

static int parport_khz(int khz, int *jtag_speed)
{
    //if (khz == 0) {
        //LOG_DEBUG("RCLK not supported");
        //return ERROR_FAIL;
    //}

    //*jtag_speed = 499999 / (khz * parport_toggling_time_ns);
    *jtag_speed = khz;
    return ERROR_OK;
}

static int parport_speed_div(int speed, int *khz)
{
    //uint32_t denominator = (speed + 1) * parport_toggling_time_ns;

    //*khz = (499999 + denominator) / denominator;
    *khz = speed;
    return ERROR_OK;
}


static struct bitbang_interface parport_bitbang = {
        .read = &parport_read,
        .write = &parport_write,
        .reset = &parport_reset,
        .blink = &parport_led,
    };

static int parport_init(void)
{
    struct cable *cur_cable;
    cur_cable = cables;

    bbhead=0;

    /* Initialize and find device */
    if (ftdi_init(&ftdi) < 0)
    {
        LOG_ERROR("ftdi_init failed");
        return ERROR_JTAG_INIT_FAILED;
    }

    if (ftdi_usb_open(&ftdi, 0x0403, 0x6001) < 0)
    {
        LOG_ERROR("ftdi_usb_open failed");
        return ERROR_JTAG_INIT_FAILED;
    }

    if (ftdi_set_bitmode(&ftdi,
        (1<<BIT_TXD_TDI )|
        (1<<BIT_DTR_TMS )|
        (1<<BIT_RTS_TCK )|
        (1<<BIT_RI__TRST)|
        (1<<BIT_DSR_SRST)
        ,BITMODE_BITBANG) < 0)
    {
        LOG_ERROR("ftdi_set_bitmode failed\n");
        return ERROR_JTAG_INIT_FAILED;
    }

    parport_cable = "jtagproxyftdi";

    while (cur_cable->name) {
        if (strcmp(cur_cable->name, parport_cable) == 0) {
            cable = cur_cable;
            break;
        }
        cur_cable++;
    }

    if (!cable) {
        LOG_ERROR("No matching cable found for %s", parport_cable);
        return ERROR_JTAG_INIT_FAILED;
    }

    dataport_value = cable->PORT_INIT;

    parport_reset(0, 0);
    parport_write(0, 0, 0);
    parport_led(1);

    bitbang_interface = &parport_bitbang;

    return ERROR_OK;
}

static int parport_quit(void)
{
    parport_led(0);

    if (parport_exit) {
        dataport_value = cable->PORT_EXIT;
        parport_write_data();
    }

    if (parport_cable) {
        free(parport_cable);
        parport_cable = NULL;
    }

    if (ftdi_set_bitmode(&ftdi, 0xFF, BITMODE_RESET) < 0)
    {
        LOG_ERROR("ftdi_set_bitmode failed");
    }
    ftdi_usb_close(&ftdi);
    ftdi_deinit(&ftdi);

    return ERROR_OK;
}

COMMAND_HANDLER(parport_handle_parport_port_command)
{
    if (CMD_ARGC == 1) {
        /* only if the port wasn't overwritten by cmdline */
        if (parport_port == 0)
            COMMAND_PARSE_NUMBER(u16, CMD_ARGV[0], parport_port);
        else {
            LOG_ERROR("The parport port was already configured!");
            return ERROR_FAIL;
        }
    }

    command_print(CMD_CTX, "parport port = 0x%" PRIx16 "", parport_port);

    return ERROR_OK;
}

COMMAND_HANDLER(parport_handle_parport_cable_command)
{
    if (CMD_ARGC == 0)
        return ERROR_OK;

    /* only if the cable name wasn't overwritten by cmdline */
    if (parport_cable == 0) {
        /* REVISIT first verify that it's listed in cables[] ... */
        parport_cable = malloc(strlen(CMD_ARGV[0]) + sizeof(char));
        strcpy(parport_cable, CMD_ARGV[0]);
    }

    /* REVISIT it's probably worth returning the current value ... */

    return ERROR_OK;
}

COMMAND_HANDLER(parport_handle_write_on_exit_command)
{
    if (CMD_ARGC != 1)
        return ERROR_COMMAND_SYNTAX_ERROR;

    COMMAND_PARSE_ON_OFF(CMD_ARGV[0], parport_exit);

    return ERROR_OK;
}

COMMAND_HANDLER(parport_handle_parport_toggling_time_command)
{
    //if (CMD_ARGC == 1) {
        //uint32_t ns;
        //int retval = parse_u32(CMD_ARGV[0], &ns);

        //if (ERROR_OK != retval)
            //return retval;

        //if (ns == 0) {
            //LOG_ERROR("0 ns is not a valid parport toggling time");
            //return ERROR_FAIL;
        //}

        //parport_toggling_time_ns = ns;
        //retval = jtag_get_speed(&wait_states);
        //if (retval != ERROR_OK)
            //return retval;
    //}

    //command_print(CMD_CTX, "parport toggling time = %" PRIu32 " ns",
            //parport_toggling_time_ns);

    return ERROR_OK;
}

static const struct command_registration parport_command_handlers[] = {
    {
        .name = "parport_port",
        .handler = parport_handle_parport_port_command,
        .mode = COMMAND_CONFIG,
        .help = "Display the address of the I/O port (e.g. 0x378) "
            "or the number of the '/dev/parport' device used.  "
            "If a parameter is provided, first change that port.",
        .usage = "[port_number]",
    },
    {
        .name = "parport_cable",
        .handler = parport_handle_parport_cable_command,
        .mode = COMMAND_CONFIG,
        .help = "Set the layout of the parallel port cable "
            "used to connect to the target.",
        /* REVISIT there's no way to list layouts we know ... */
        .usage = "[layout]",
    },
    {
        .name = "parport_write_on_exit",
        .handler = parport_handle_write_on_exit_command,
        .mode = COMMAND_CONFIG,
        .help = "Configure the parallel driver to write "
            "a known value to the parallel interface on exit.",
        .usage = "('on'|'off')",
    },
    {
        .name = "parport_toggling_time",
        .handler = parport_handle_parport_toggling_time_command,
        .mode = COMMAND_CONFIG,
        .help = "Displays or assigns how many nanoseconds it "
            "takes for the hardware to toggle TCK.",
        .usage = "[nanoseconds]",
    },
    COMMAND_REGISTRATION_DONE
};

struct jtag_interface parport_interface = {
    .name = "parport",
    .supported = DEBUG_CAP_TMS_SEQ,
    .commands = parport_command_handlers,

    .init = parport_init,
    .quit = parport_quit,
    .khz = parport_khz,
    .speed_div = parport_speed_div,
    .speed = parport_speed,
    .execute_queue = bitbang_execute_queue,
};
