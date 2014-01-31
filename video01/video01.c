
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// 2  outer corner
// 4
// 6
// 8  TX out
// 10 RX in

// The raspberry pi firmware at the time this was written defaults
// loading at address 0x8000.  Although this bootloader could easily
// load at 0x0000, it loads at 0x8000 so that the same binaries built
// for the SD card work with this bootloader.  Change the ARMBASE
// below to use a different location.

#include "image_data.h"

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int GETPC ( void );
extern void BRANCHTO ( unsigned int );
extern void dummy ( unsigned int );

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


unsigned int MailboxWrite ( unsigned int fbinfo_addr, unsigned int channel )
{
    unsigned int mailbox;

    mailbox=0x2000B880;
    while(1)
    {
        if((GET32(mailbox+0x18)&0x80000000)==0) break;
    }
    PUT32(mailbox+0x20,fbinfo_addr+channel);
    return(0);
}

unsigned int MailboxRead ( unsigned int channel )
{
    unsigned int ra;
    unsigned int mailbox;

    mailbox=0x2000B880;
    while(1)
    {
        while(1)
        {
            ra=GET32(mailbox+0x18);
            if((ra&0x40000000)==0) break;
        }
        //hexstrings(ra);
        ra=GET32(mailbox+0x00);
        //hexstring(ra);
        if((ra&0xF)==channel) break;
    }
    return(ra);
}


//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra,rb;
    unsigned int ry,rx;

    uart_init();
    hexstring(0x12345678);
    hexstring(GETPC());
    timer_init();

    PUT32(0x40040000, 640); /* #0 Physical Width */
    PUT32(0x40040004, 480); /* #4 Physical Height */
    PUT32(0x40040008, 640); /* #8 Virtual Width */
    PUT32(0x4004000C, 480); /* #12 Virtual Height */
    PUT32(0x40040010, 0); /* #16 GPU - Pitch */
    PUT32(0x40040014, 32); /* #20 Bit Depth */
    PUT32(0x40040018, 0); /* #24 X */
    PUT32(0x4004001C, 0); /* #28 Y */
    PUT32(0x40040020, 0); /* #32 GPU - Pointer */
    PUT32(0x40040024, 0); /* #36 GPU - Size */


    hexstring(MailboxWrite(0x40040000,1));
    hexstring(MailboxRead(1));
    rb=0x40040000;
    for(ra=0;ra<10;ra++)
    {
        hexstrings(rb); hexstring(GET32(rb));
        rb+=4;
    }

    rb=GET32(0x40040020);
    hexstring(rb);
    for(ra=0;ra<10000;ra++)
    {
        PUT32(rb,~((ra&0xFF)<<0));
        rb+=4;
    }
    for(ra=0;ra<10000;ra++)
    {
        PUT32(rb,~((ra&0xFF)<<8));
        rb+=4;
    }
    for(ra=0;ra<10000;ra++)
    {
        PUT32(rb,~((ra&0xFF)<<16));
        rb+=4;
    }
    for(ra=0;ra<10000;ra++)
    {
        PUT32(rb,~((ra&0xFF)<<24));
        rb+=4;
    }
    rb=GET32(0x40040020);
    hexstring(rb);
    ra=0;
    for(ry=0;ry<480;ry++)
    {
        for(rx=0;rx<480;rx++)
        {
            PUT32(rb,image_data[ra++]);
            rb+=4;
        }
        for(;rx<640;rx++)
        {
            PUT32(rb,0);
            rb+=4;
        }
    }

    return(0);
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

