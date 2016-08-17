
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// 2  outer corner sd card end
// 4
// 6
// 8  TX out
// 10 RX in

//inner corner
// 1 3.3v
// 3 GPIO2 SDA1
// 5 GPIO3 SCL1
// 7 GPOI4
// 9 Ground

// vcc gnd scl sda



extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int GETPC ( void );
extern void dummy ( unsigned int );
extern unsigned int BRANCHTO ( unsigned int );

extern void uart_init ( void );
extern unsigned int uart_lcr ( void );
extern void uart_flush ( void );
extern void uart_send ( unsigned int );
extern unsigned int uart_recv ( void );
extern unsigned int uart_check ( void );
extern void hexstring ( unsigned int );
extern void hexstrings ( unsigned int );
extern void timer_init ( void );
extern unsigned int timer_tick ( void );

extern void timer_init ( void );
extern unsigned int timer_tick ( void );



//count up timer
#define ARM_TIMER_LOD 0x2000B400
#define ARM_TIMER_VAL 0x2000B404
#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_DIV 0x2000B41C
#define ARM_TIMER_CNT 0x2000B420

#define GPFSEL0 0x20200000
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPLEV0  0x20200034


#define PADDR 0x78


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



unsigned int hex_screen_history[8];


#include "fontdata.h"



#define SDA 2
#define SCL 3


//625 ticks of a 250Mhz clock is the minimum count for 400mhz basically.
//2.5us * 250mhz = 625  does that sound right?
//156.25 ticks per quarter cycle.
//------------------------------------------------------------------------
static void i2c_delay ( void )
{
    unsigned int beg,end;

    beg=GET32(ARM_TIMER_CNT);
    while(1)
    {
        end=GET32(ARM_TIMER_CNT);
        if((end-beg)>400) break;
    }
}
//------------------------------------------------------------------------
static void scl_high ( void )
{
    PUT32(GPSET0,1<<SCL);
}
//------------------------------------------------------------------------
static void sda_high ( void )
{
    PUT32(GPSET0,1<<SDA);
}
//------------------------------------------------------------------------
static void scl_low ( void )
{
    PUT32(GPCLR0,1<<SCL);
}
//------------------------------------------------------------------------
static void sda_low ( void )
{
    PUT32(GPCLR0,1<<SDA);
}
//------------------------------------------------------------------------
static unsigned int sda_read ( void )
{
    unsigned int ra;

    ra=GET32(GPLEV0);
    ra&=(1<<SDA);
    return(ra);
}
//------------------------------------------------------------------------
static void sda_input ( void )
{
    unsigned int ra;

    ra=GET32(GPFSEL0);
    ra&=~(7<<(SDA*3));
    //ra|=1<<(SDA*3);
    PUT32(GPFSEL0,ra);
}
//------------------------------------------------------------------------
static void sda_output ( void )
{
    unsigned int ra;

    ra=GET32(GPFSEL0);
    ra&=~(7<<(SDA*3));
    ra|=1<<(SDA*3);
    PUT32(GPFSEL0,ra);
}
//------------------------------------------------------------------------
static void i2c_start ( void )
{
    i2c_delay();
    i2c_delay();
    sda_low();
    i2c_delay();
    i2c_delay();
    scl_low();
    i2c_delay();
}
//------------------------------------------------------------------------
static void i2c_stop ( void )
{
    i2c_delay();
    scl_high();
    i2c_delay();
    sda_high();
    i2c_delay();
    i2c_delay();
}
//------------------------------------------------------------------------
unsigned int i2c_write_byte ( unsigned int b )
{
    unsigned int ra;
    for(ra=0x80;ra;ra>>=1)
    {
        i2c_delay();
        if(ra&b) sda_high();
        else     sda_low();
        i2c_delay();
        scl_high();
        i2c_delay();
        scl_low();
        i2c_delay();
        sda_low();
        i2c_delay();
    }
    i2c_delay();
    sda_input();
    i2c_delay();
    scl_high();
    i2c_delay();
    ra=sda_read();
    i2c_delay();
    scl_low();
    i2c_delay();
    sda_output();
    i2c_delay();
    return(ra);
}
//------------------------------------------------------------------------
unsigned int i2c_read_byte ( unsigned int *b )
{
    unsigned int ra;
    unsigned int rb;
    i2c_delay();
    sda_input();
    rb=0;
    for(ra=0;ra<9;ra++)
    {
        i2c_delay();
        scl_high();
        i2c_delay();
        rb<<=1;
        if(sda_read()) rb|=1;
        i2c_delay();
        scl_low();
        i2c_delay();
    }
    sda_output();
    i2c_delay();
    ra=rb&1;
    *b=rb>>1;
    return(ra);
}
//------------------------------------------------------------------------
void send_command ( unsigned int cmd )
{
//    PUT32(GPCLR0,1<<25); //D/C = 0 for command
    //spi_one_byte(cmd);
    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        hexstring(0xBADBAD00);
        return;
    }
    //not continuing
    //want D/C a zero
    if(i2c_write_byte(0x00))
    {
        //i2c_stop();
        //hexstring(0xBADBAD00);
        //return;
    }
    if(i2c_write_byte(cmd))
    {
        //i2c_stop();
        //hexstring(0xBADBAD00);
        //return;
    }
    i2c_stop();
}
//------------------------------------------------------------------------
void send_data ( unsigned int data )
{
    //PUT32(GPSET0,1<<25); //D/C = 1 for data
    //spi_one_byte(data);
    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        hexstring(0xBADBAD00);
        return;
    }
    //not continuing
    //want D/C a one
    if(i2c_write_byte(0x40))
    {
        //i2c_stop();
        //hexstring(0xBADBAD00);
        //return;
    }
    if(i2c_write_byte(data))
    {
        //i2c_stop();
        //hexstring(0xBADBAD00);
        //return;
    }
    i2c_stop();
}
//------------------------------------------------------------------------
void SetPageStart ( unsigned int x )
{
    send_command(0xB0|(x&0x07));
}
//------------------------------------------------------------------------
void SetColumn ( unsigned int x )
{
    //x+=0x20;
    send_command(0x10|((x>>4)&0x0F));
    send_command(0x00|((x>>0)&0x0F));
}
//------------------------------------------------------------------------
void show_text(unsigned int col, char *s)
{
    unsigned int ra;

    SetPageStart(col);
    SetColumn(0);

    while(*s)
    {
        for(ra=0;ra<8;ra++) send_data(fontdata[(unsigned)(*s)][ra]);
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
        for(rc=0;rc<8;rc++) send_data(fontdata[rb][rc]);
    }
}
//------------------------------------------------------------------------
void hex_screen ( unsigned int x )
{
    unsigned int ra;

    for(ra=0;ra<7;ra++)
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
        for(rb=0;rb<0x80;rb++) send_data(0);
    }
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    uart_init();
    hexstring(0x12345678);

    PUT32(ARM_TIMER_CTL,0x00000000);
    PUT32(ARM_TIMER_CTL,0x00000200);

    ra=GET32(GPFSEL0);
    ra&=~(7<<(SDA*3));
    ra&=~(7<<(SCL*3));
    ra|=1<<(SDA*3);
    ra|=1<<(SCL*3);
    PUT32(GPFSEL0,ra);

    scl_high();
    sda_high();
    i2c_delay();
    i2c_delay();
    i2c_delay();

    ////find who acks
    //for(ra=0;ra<0x100;ra+=2)
    //{
        //unsigned int rb;
        ////hexstrings(ra);
        //i2c_start();
        //rb=i2c_write_byte(ra|0);
        //i2c_stop();
        ////hexstring(rb);
        //if(rb==0) hexstring(ra);
    //}
    ////0x78 is the address

if(1)
{

    // Display Init sequence for 64x48 OLED module
    send_command(DISPLAYOFF);           // 0xAE
    send_command(SETDISPLAYCLOCKDIV);   // 0xD5
    send_command(0x80);                 // the suggested ratio 0x80
    send_command(SETMULTIPLEX);         // 0xA8
    send_command(0x3F);
    send_command(SETDISPLAYOFFSET);     // 0xD3
    send_command(0x0);                  // no offset
    send_command(SETSTARTLINE | 0x0);   // line #0
    send_command(CHARGEPUMP);           // enable charge pump
    send_command(0x14);
    send_command(MEMORYMODE);
    send_command(0x02);
    send_command(SEGREMAP | 0x1);
    send_command(COMSCANDEC);
    send_command(SETCOMPINS);           // 0xDA
    send_command(0x12);
    send_command(SETCONTRAST);          // 0x81
    send_command(0x7F);
    send_command(SETPRECHARGE);         // 0xd9
    send_command(0xF1);
    send_command(SETVCOMDESELECT);          // 0xDB
    send_command(0x40);
    send_command(NORMALDISPLAY);            // 0xA6
    send_command(DISPLAYALLONRESUME);   // 0xA4
    ClearScreen();
    send_command(DISPLAYON);                //--turn on oled panel
    //send_command(DISPLAYALLON);

    show_text(0,"HELLO");
    show_text(1,"World!");
    show_text_hex(2,0x12345678);
    show_text_hex(3,0xABCDEF00);
    for(ra=0;ra<=0x200;ra++) show_text_hex(5,ra);

    //ClearScreen();
    for(ra=0;ra<8;ra++) hex_screen_history[ra]=0;
    for(ra=0;ra<0x1000;ra++) hex_screen(ra);
}
    hexstring(0x12341234);

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
