
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#include "blinker.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define GPFSEL0 0x20200000
#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

#define AUX_SPI0_CS     0x20204000
#define AUX_SPI0_FIFO   0x20204004
#define AUX_SPI0_CLK    0x20204008
#define AUX_SPI0_DLEN   0x2020400C
#define AUX_SPI0_LTOH   0x20204010
#define AUX_SPI0_DC     0x20204014



//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0

//((250,000,000/115200)/8)-1 = 270
//------------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    ra=GET32(AUX_ENABLES);
    ra|=1; //enable mini uart
    PUT32(AUX_ENABLES,ra);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,270);
    //setup gpio before enabling uart
    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    PUT32(GPFSEL1,ra);
    PUT32(GPPUD,0);
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);
    //enable uart
    PUT32(AUX_MU_CNTL_REG,3);
}
//------------------------------------------------------------------------
void uart_putc ( unsigned int c )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;
    }
    PUT32(AUX_MU_IO_REG,c);
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}

//GPIO7  SPI0_CE1_N  P1-26
//GPIO8  SPI0_CE0_N  P1-24
//GPIO9  SPI0_MISO   P1-21
//GPIO10 SPI0_MOSI   P1-19
//GPIO11 SPI0_SCLK   P1-23
//alt function 0 for all of the above

//P1 1  +3V3
//P1 25  GND
//p1 26  GPIO_CE1







//------------------------------------------------------------------------
void spi_init ( void )
{
    unsigned int ra;

    ra=GET32(AUX_ENABLES);
    ra|=2; //enable spi0
    PUT32(AUX_ENABLES,ra);


    ra=GET32(GPFSEL0);
    ra&=~(7<<27); //gpio9
    ra|=4<<27;    //alt0
    ra&=~(7<<24); //gpio8
    ra|=4<<24;    //alt0
    ra&=~(7<<21); //gpio7
    ra|=4<<21;    //alt0
    PUT32(GPFSEL0,ra);
    ra=GET32(GPFSEL1);
    ra&=~(7<<0); //gpio10/
    ra|=4<<0;    //alt0
    ra&=~(7<<3); //gpio11/
    ra|=4<<3;    //alt0
    PUT32(GPFSEL1,ra);

    PUT32(AUX_SPI0_CS,0x0000030);
    PUT32(AUX_SPI0_CLK,0x0000); //slowest possible, could probably go faster here



}
//------------------------------------------------------------------------
unsigned int pdi_command ( unsigned int cmd )
{
    unsigned int data;

    PUT32(AUX_SPI0_CS,0x004000B0); //TA=1 cs asserted
    while(1)
    {
        if(GET32(AUX_SPI0_CS)&(1<<18)) break; //TXD
    }
    PUT32(AUX_SPI0_FIFO,(cmd>>24)&0xFF);
    PUT32(AUX_SPI0_FIFO,(cmd>>16)&0xFF);
    PUT32(AUX_SPI0_FIFO,(cmd>> 8)&0xFF);
    PUT32(AUX_SPI0_FIFO,(cmd>> 0)&0xFF);
    while(1) if(GET32(AUX_SPI0_CS)&(1<<16)) break;
    while(1) if(GET32(AUX_SPI0_CS)&(1<<17)) break;
    data=GET32(AUX_SPI0_FIFO)&0xFF;
    while(1) if(GET32(AUX_SPI0_CS)&(1<<17)) break;
    data<<=8;
    data|=GET32(AUX_SPI0_FIFO)&0xFF;
    while(1) if(GET32(AUX_SPI0_CS)&(1<<17)) break;
    data<<=8;
    data|=GET32(AUX_SPI0_FIFO)&0xFF;
    while(1) if(GET32(AUX_SPI0_CS)&(1<<17)) break;
    data<<=8;
    data|=GET32(AUX_SPI0_FIFO)&0xFF;
    PUT32(AUX_SPI0_CS,0x00400000); //cs0 comes back up
    return(data);
}
//------------------------------------------------------------------------
int prog_avr ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    unsigned int pages;
    unsigned int psize;
    unsigned int poff;




    ra=pdi_command(0xAC530000);
    if((ra&0x0000FF00)!=0x00005300)
    {
        hexstring(0xBAD);
        return(1);
    }
    rb=0;
    ra=pdi_command(0x30000000);
    rb<<=8; rb|=ra&0xFF;
    ra=pdi_command(0x30000100);
    rb<<=8; rb|=ra&0xFF;
    ra=pdi_command(0x30000200);
    rb<<=8; rb|=ra&0xFF;
    hexstring(rb);
    if(rb!=0x001E9587)
    {
        //not really an error, this code is written for the atmega32u4
        //should be easy to adapt to another part.
        hexstring(0xBAD);
        return(1);
    }
    for(ra=0;ra<10;ra++)
    {
        rb=pdi_command(0x28000000|(ra<<8));
        rc=pdi_command(0x20000000|(ra<<8));
        hexstring((ra<<16)|((rb&0xFF)<<8)|(rc&0xFF));
    }

    pdi_command(0xAC800000); //chip erase
    PUT32(AUX_SPI0_CS,0x00000000); //deassert reset
for(ra=0;ra<0x100000;ra++) dummy(ra);
    PUT32(AUX_SPI0_CS,0x00400000); //assert reset
for(ra=0;ra<0x10000;ra++) dummy(ra);

    ra=pdi_command(0xAC530000);
    hexstring(ra);
    if((ra&0x0000FF00)!=0x00005300)
    {
        hexstring(0xBAD);
        return(1);
    }
    rb=0;
    ra=pdi_command(0x30000000);
    rb<<=8; rb|=ra&0xFF;
    ra=pdi_command(0x30000100);
    rb<<=8; rb|=ra&0xFF;
    ra=pdi_command(0x30000200);
    rb<<=8; rb|=ra&0xFF;
    hexstring(rb);
    if(rb!=0x001E9587)
    {
        //not really an error, this code is written for the atmega32u4
        //should be easy to adapt to another part.
        hexstring(0xBAD);
        return(1);
    }
    for(ra=0;ra<10;ra++)
    {
        rb=pdi_command(0x28000000|(ra<<8));
        rc=pdi_command(0x20000000|(ra<<8));
        hexstring((ra<<16)|((rb&0xFF)<<8)|(rc&0xFF));
    }

    psize=sizeof(rom);
    psize>>=1;
    pages=psize>>6;

    hexstring(psize);
    hexstring(pages);

    poff=0;
    //for(np=0;np<pages;np++)
    {
        for(ra=0;ra<64;ra++)
        {
//            if(poff>=psize) break;
            pdi_command(0x40000000|(ra<<8)|((rom[poff]>>0)&0xFF)); //low first
            pdi_command(0x48000000|(ra<<8)|((rom[poff]>>8)&0xFF)); //then high
            poff++;
        }
        //for(;ra<64;ra++)
        //{
            //pdi_command(0x400000FF|(ra<<8)); //low first
            //pdi_command(0x480000FF|(ra<<8)); //then high
            ////poff++;
        //}
        pdi_command(0x4D000000);
        pdi_command(0x4C000000);//|(np<<14));
for(ra=0;ra<0x10000;ra++) dummy(ra);
    }
    for(ra=0;ra<10;ra++)
    {
        rb=pdi_command(0x28000000|(ra<<8));
        rc=pdi_command(0x20000000|(ra<<8));
        hexstring((ra<<16)|((rb&0xFF)<<8)|(rc&0xFF));
    }

    ra=pdi_command(0x50000000); hexstring(ra);
    ra=pdi_command(0x58080000); hexstring(ra);
    ra=pdi_command(0x50080000); hexstring(ra);

    return(0);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    uart_init();
    hexstring(0x12345678);
    for(ra=0;ra<10;ra++)
    {
        hexstring(ra);
    }
    spi_init();
    PUT32(AUX_SPI0_CS,0x00400000); //cs1 low, reset
    for(ra=0;ra<0x10000;ra++) dummy(ra);
    prog_avr();
    PUT32(AUX_SPI0_CS,0x00000000); //cs1 comes back up
    hexstring(0x12345678);
    return(0);
}//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2014 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
