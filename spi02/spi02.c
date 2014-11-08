
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#include "fontdata.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define GPFSEL0 0x20200000
#define GPFSEL1 0x20200004
#define GPFSEL2 0x20200008

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


#define SETCONTRAST         0x81
#define DISPLAYALLONRESUME  0xA4
#define DISPLAYALLON        0xA5
#define NORMALDISPLAY       0xA6
#define INVERTDISPLAY       0xA7
#define DISPLAYOFF          0xAE
#define DISPLAYON           0xAF
#define SETDISPLAYOFFSET    0xD3
#define SETCOMPINS          0xDA
#define SETVCOMDESELECT     0xDB
#define SETDISPLAYCLOCKDIV  0xD5
#define SETPRECHARGE        0xD9
#define SETMULTIPLEX        0xA8
#define SETLOWCOLUMN        0x00
#define SETHIGHCOLUMN       0x10
#define SETSTARTLINE        0x40
#define MEMORYMODE          0x20
#define COMSCANINC          0xC0
#define COMSCANDEC          0xC8
#define SEGREMAP            0xA0
#define CHARGEPUMP          0x8D
#define EXTERNALVCC         0x01
#define SWITCHCAPVCC        0x02



unsigned int hex_screen_history[6];

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

//GPIO7  SPI0_CE1_N  P1-26  (use for reset)
//GPIO8  SPI0_CE0_N  P1-24
//GPIO9  SPI0_MISO   P1-21
//GPIO10 SPI0_MOSI   P1-19
//GPIO11 SPI0_SCLK   P1-23
//alt function 0 for all of the above

//P1 1  +3V3
//P1 25  GND

//P1 22 GPIO25  use as D/C


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
    ra=GET32(GPFSEL2);
    ra&=~(7<<15); //gpio25/
    ra|=1<<15;    //output
    PUT32(GPFSEL2,ra);

    PUT32(AUX_SPI0_CS,0x0000030);
//    PUT32(AUX_SPI0_CLK,0x0000); //slowest possible, could probably go faster here
    PUT32(AUX_SPI0_CLK,26);

}
//------------------------------------------------------------------------
void spi_one_byte ( unsigned int x )
{
    PUT32(AUX_SPI0_CS,0x000000B0); //TA=1 cs asserted
    while(1)
    {
        if(GET32(AUX_SPI0_CS)&(1<<18)) break; //TXD
    }
    PUT32(AUX_SPI0_FIFO,x&0xFF);
    while(1) if(GET32(AUX_SPI0_CS)&(1<<16)) break;
    //while(1) if(GET32(AUX_SPI0_CS)&(1<<17)) break; //should I wait for this?
    PUT32(AUX_SPI0_CS,0x00000000); //cs0 comes back up
}
//------------------------------------------------------------------------
void spi_command ( unsigned int cmd )
{
    PUT32(GPCLR0,1<<25); //D/C = 0 for command
    spi_one_byte(cmd);
}
//------------------------------------------------------------------------
void spi_data ( unsigned int data )
{
    PUT32(GPSET0,1<<25); //D/C = 1 for data
    spi_one_byte(data);
}
//------------------------------------------------------------------------
void SetPageStart ( unsigned int x )
{
    spi_command(0xB0|(x&0x07));
}
//------------------------------------------------------------------------
void SetColumn ( unsigned int x )
{
    x+=0x20;
    spi_command(0x10|((x>>4)&0x0F));
    spi_command(0x00|((x>>0)&0x0F));
}
//------------------------------------------------------------------------
void show_text(unsigned int col, char *s)
{
    unsigned int ra;

    SetPageStart(col);
    SetColumn(0);

    while(*s)
    {
        for(ra=0;ra<8;ra++) spi_data(fontdata[(unsigned)(*s)][ra]);
        s++;
    }
}
//------------------------------------------------------------------------
void show_text_hex(unsigned int col, unsigned int x)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    SetPageStart(col);
    SetColumn(0);

    for(ra=0;ra<8;ra++)
    {
        rb=(x>>28)&0xF;
        if(rb>9) rb+=0x37; else rb+=0x30;
        x<<=4;
        for(rc=0;rc<8;rc++) spi_data(fontdata[rb][rc]);
    }
}
//------------------------------------------------------------------------
void hex_screen ( unsigned int x )
{
    unsigned int ra;

    for(ra=0;ra<5;ra++)
    {
        hex_screen_history[ra]=hex_screen_history[ra+1];
        show_text_hex(ra,hex_screen_history[ra]);
    }
    hex_screen_history[ra]=x;
    show_text_hex(ra,hex_screen_history[ra]);
}
//------------------------------------------------------------------------
void ClearScreen ( void )
{
    unsigned int ra;
    unsigned int rb;

    for(ra=0;ra<8;ra++)
    {
        SetPageStart(ra);
        SetColumn(0);
        for(rb=0;rb<0x80;rb++) spi_data(0);
    }
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

    PUT32(AUX_SPI0_CS,0x00000000); //cs1 high
    for(ra=0;ra<0x10000;ra++) dummy(ra);
    PUT32(AUX_SPI0_CS,0x00400000); //cs1 low, reset
    for(ra=0;ra<0x10000;ra++) dummy(ra);
    PUT32(AUX_SPI0_CS,0x00000000); //cs1 comes back up


    // Display Init sequence for 64x48 OLED module
    spi_command(DISPLAYOFF);           // 0xAE
    spi_command(SETDISPLAYCLOCKDIV);   // 0xD5
    spi_command(0x80);                 // the suggested ratio 0x80
    spi_command(SETMULTIPLEX);         // 0xA8
    spi_command(0x2F);
    spi_command(SETDISPLAYOFFSET);     // 0xD3
    spi_command(0x0);                  // no offset
    spi_command(SETSTARTLINE | 0x0);   // line #0
    spi_command(CHARGEPUMP);           // enable charge pump
    spi_command(0x14);
    spi_command(NORMALDISPLAY);            // 0xA6
    spi_command(DISPLAYALLONRESUME);   // 0xA4
    spi_command(SEGREMAP | 0x1);
    spi_command(COMSCANDEC);
    spi_command(SETCOMPINS);           // 0xDA
    spi_command(0x12);
    spi_command(SETCONTRAST);          // 0x81
    spi_command(0x7F);
    spi_command(SETPRECHARGE);         // 0xd9
    spi_command(0xF1);
    spi_command(SETVCOMDESELECT);          // 0xDB
    spi_command(0x40);
    ClearScreen();
    spi_command(DISPLAYON);                //--turn on oled panel
    //spi_command(DISPLAYALLON);

    show_text(0,"HELLO");
    show_text(1,"World!");
    show_text_hex(2,0x12345678);
    show_text_hex(3,0xABCDEF00);
    for(ra=0;ra<=0x10000;ra++) show_text_hex(5,ra);

    //ClearScreen();
    for(ra=0;ra<6;ra++) hex_screen_history[ra]=0;
    for(ra=0;ra<0x1000;ra++) hex_screen(ra);

    hexstring(0x12345678);
    return(0);
}
//-------------------------------------------------------------------------
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
