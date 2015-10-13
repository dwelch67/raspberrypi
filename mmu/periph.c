
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void BRANCHTO ( unsigned int );
extern void dummy ( unsigned int );

#define SYSTIMERCLO     (0x20003004)

#define GPFSEL1         (0x20200004)
#define GPSET0          (0x2020001C)
#define GPCLR0          (0x20200028)
#define GPPUD           (0x20200094)
#define GPPUDCLK0       (0x20200098)

#define AUX_ENABLES     (0x20215004)
#define AUX_MU_IO_REG   (0x20215040)
#define AUX_MU_IER_REG  (0x20215044)
#define AUX_MU_IIR_REG  (0x20215048)
#define AUX_MU_LCR_REG  (0x2021504C)
#define AUX_MU_MCR_REG  (0x20215050)
#define AUX_MU_LSR_REG  (0x20215054)
#define AUX_MU_MSR_REG  (0x20215058)
#define AUX_MU_SCRATCH  (0x2021505C)
#define AUX_MU_CNTL_REG (0x20215060)
#define AUX_MU_STAT_REG (0x20215064)
#define AUX_MU_BAUD_REG (0x20215068)

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//------------------------------------------------------------------------
unsigned int uart_lcr ( void )
{
    return(GET32(AUX_MU_LSR_REG));
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x01) break;
    }
    return(GET32(AUX_MU_IO_REG)&0xFF);
}
//------------------------------------------------------------------------
unsigned int uart_check ( void )
{
    if(GET32(AUX_MU_LSR_REG)&0x01) return(1);
    return(0);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int c )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;
    }
    PUT32(AUX_MU_IO_REG,c);
}
//------------------------------------------------------------------------
void uart_flush ( void )
{
    while(1)
    {
        if((GET32(AUX_MU_LSR_REG)&0x100)==0) break;
    }
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
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    PUT32(AUX_ENABLES,1);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,270);
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
    PUT32(AUX_MU_CNTL_REG,3);
}
//-------------------------------------------------------------------------
unsigned int system_timer_low ( void )
{
    return(GET32(SYSTIMERCLO));
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
