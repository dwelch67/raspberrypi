
//-------------------------------------------------------------------
//-------------------------------------------------------------------

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

// vcc data clock ground

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

#define SDA 2
#define SCL 3

//625 ticks of a 250Mhz clock is the minimum count for 400khz basically.
//2.5us * 250mhz = 625  does that sound right?
//156.25 ticks per quarter cycle.
//-------------------------------------------------------------------

static void swd_delay ( void )
{
    unsigned int beg,end;

    beg=GET32(ARM_TIMER_CNT);
    while(1)
    {
        end=GET32(ARM_TIMER_CNT);
        if((end-beg)>150) break;
    }
}
//-------------------------------------------------------------------
static void scl_high ( void )
{
    PUT32(GPSET0,1<<SCL);
}
//-------------------------------------------------------------------
static void sda_high ( void )
{
    PUT32(GPSET0,1<<SDA);
}
//-------------------------------------------------------------------
static void scl_low ( void )
{
    PUT32(GPCLR0,1<<SCL);
}
//-------------------------------------------------------------------
static void sda_low ( void )
{
    PUT32(GPCLR0,1<<SDA);
}
//-------------------------------------------------------------------
static unsigned int sda_read ( void )
{
    unsigned int ra;

    ra=GET32(GPLEV0);
    ra&=(1<<SDA);
    return(ra);
}
//-------------------------------------------------------------------
static void sda_input ( void )
{
    unsigned int ra;

    ra=GET32(GPFSEL0);
    ra&=~(7<<(SDA*3));
    //ra|=1<<(SDA*3);
    PUT32(GPFSEL0,ra);
}
//-------------------------------------------------------------------
static void sda_output ( void )
{
    unsigned int ra;

    ra=GET32(GPFSEL0);
    ra&=~(7<<(SDA*3));
    ra|=1<<(SDA*3);
    PUT32(GPFSEL0,ra);
}
//-------------------------------------------------------------------
static void scl_strobe ( void )
{
    swd_delay();
    scl_high();
    swd_delay();
    scl_low();
    swd_delay();
}
//-------------------------------------------------------------------
int swd_read ( unsigned int request, unsigned int *response )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rx;

    sda_output();
    sda_low();
    scl_strobe();
    scl_strobe();

    rx=request;
    for(ra=0;ra<8;ra++)
    {
        if(rx&0x80) sda_high();
        else        sda_low();
        rx<<=1;
        scl_strobe();
    }
    sda_input();
    rb=0;
    for(ra=0;ra<3;ra++)
    {
        scl_strobe();
        rb>>=1;
        if(sda_read()) rb|=4;
    }
    if(rb!=0x01)
    {
        //0x02 wait
        //0x04 fault
        sda_output();
        sda_high();
        for(ra=0;ra<50;ra++) scl_strobe();
        hexstrings(rb);
        hexstring(0xBAD1111);
        return(1);
    }
    rb=0;
    rc=0;
    rx=0;
    for(ra=0;ra<32;ra++)
    {
        scl_strobe();
        rc>>=1;
        if(sda_read())
        {
                rc|=0x80000000;
                rx++;
        }
    }
    scl_strobe();
    if(sda_read()) rb=1;
    sda_output();
    sda_low();
    scl_strobe();
    scl_strobe();
    scl_strobe();
    scl_strobe();
    if(rb!=(rx&1))
    {
        hexstring(0xBAD2222);
        return(1); //PARITY ERROR
    }
    *response=rc;
    return(0);
}
//-------------------------------------------------------------------
int swd_write ( unsigned int request, unsigned int data )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rx;

    sda_output();
    sda_low();
    scl_strobe();
    scl_strobe();
    scl_strobe();
    scl_strobe();
    scl_strobe();
    scl_strobe();

    rx=request;
    for(ra=0;ra<8;ra++)
    {
        if(rx&0x80) sda_high();
        else        sda_low();
        rx<<=1;
        scl_strobe();
    }
    sda_input();
    rb=0;
    for(ra=0;ra<3;ra++)
    {
        scl_strobe();
        rb>>=1;
        if(sda_read()) rb|=4;
    }
    if(rb!=0x01)
    {
        //0x02 wait
        //0x04 fault
        sda_output();
        sda_high();
        for(ra=0;ra<50;ra++) scl_strobe();
        hexstrings(rb);
        hexstring(0xBAD1112);
        return(1);
    }
    scl_strobe();
    sda_output();
    sda_low();
    scl_strobe();
    rc=data;
    rx=0;
    for(ra=0;ra<32;ra++)
    {
        if(rc&1) { sda_high(); rx++; }
        else     sda_low();
        rc>>=1;
        scl_strobe();
    }
    if(rx&1) sda_high();
    else     sda_low();
    scl_strobe();

    sda_low();
    scl_strobe();
    scl_strobe();
    scl_strobe();
    scl_strobe();
    return(0);
}

//start 1
//apndp 0 dp 1 ap
//rnw  0 write 1 read
//a[3]
//a[2]
//parity one if prior 4 are odd
//stop 0
//park 1

//READ DP
//1 01 00 1 01
#define DPR_IDCODE 0xA5
//1 01 01 0 01
#define DPR_STAT 0xA9
//1 01 11 1 01
#define DPR_RDBUFF 0xBD

//1 00 01 1 01
#define DPW_CTRL 0x8D


//1 00 10 1 01
#define DPW_SELECT 0x95
#define DP_SELECT_IDR 0xFC0000F0
#define DP_SELECT_CSW 0x00000000
//address bank
//0x00 0x00 CSW Control/Status Word Register
//0x04 0x00 TAR Transfer Address Register
//0x0C 0x00 DRW Data Read/Write Register
//0xFC 0x0F IDR Identification Register

//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    //unsigned int rc;
    unsigned int rx;

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
    PUT32(GPCLR0,(1<<SDA)|(1<<SCL));

    sda_output();
    sda_high();
    for(ra=0;ra<60;ra++) scl_strobe();
//0111 1001 1110 0111
    rx=0x79E7;
    for(ra=0;ra<16;ra++)
    {
        if(rx&0x8000) sda_high();
        else          sda_low();
        rx<<=1;
        scl_strobe();
    }
    sda_high();
    for(ra=0;ra<60;ra++) scl_strobe();
    if(swd_read(DPR_IDCODE,&rb)) return(1);
    hexstring(rb);

    //if(swd_write(DPW_SELECT,0x00000000)) return(1);

    if(swd_read(DPR_STAT,&rb)) return(1);
    hexstring(rb);

    if(swd_read(DPR_IDCODE,&rb)) return(1);
    hexstring(rb);



    if(swd_write(DPW_CTRL,0xF0000000)) return(1);
    if(swd_read(DPR_STAT,&rb)) return(1);
    hexstring(rb);
    if(swd_write(DPW_CTRL,0x50000000)) return(1);
    if(swd_read(DPR_STAT,&rb)) return(1);
    hexstring(rb);
    if(swd_write(DPW_CTRL,0x50000000)) return(1);
    if(swd_read(DPR_STAT,&rb)) return(1);
    hexstring(rb);
    if(swd_write(DPW_CTRL,0x50000000)) return(1);
    if(swd_read(DPR_STAT,&rb)) return(1);
    hexstring(rb);

    if(swd_read(DPR_IDCODE,&rb)) return(1);
    hexstring(rb);


    if(swd_write(DPW_SELECT,DP_SELECT_CSW)) return(1);
    if(swd_read(DPR_RDBUFF,&rb)) return(1);
    hexstring(rb);


    if(swd_write(DPW_SELECT,DP_SELECT_IDR)) return(1);
    if(swd_read(DPR_RDBUFF,&rb)) return(1);
    hexstring(rb);

    hexstring(0x12341234);

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//
// Copyright (c) 2014 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------

