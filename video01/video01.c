
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

//#include "image_data.h"
#include "functions.h"
#include "periph.h"
#include "timer.h"
#include "terminal.h"
#include "display.h"
#include "vectors.h"
#define SCREENBUF_LENGTH 100

#define MMUTABLEBASE 0x00100000

#define MMUTABLESIZE (0x8000)
#define MMUTABLEMASK ((MMUTABLESIZE-1)>>2)

#define TOP_LEVEL_WORDS (1<<((31-20)+1))
#define COARSE_TABLE_WORDS (1<<((19-12)+1))
#define SMALL_TABLE_WORDS (1<<((11-0)+1))
int multiplylookup[640];

		
static inline unsigned int min(unsigned int a, unsigned int b, unsigned int c)
{
    unsigned int m = a;
    if (m > b) m = b;
    if (m > c) m = c;
    return m;
}
static inline unsigned int max(unsigned int a, unsigned int b, unsigned int c)
{
    unsigned int m = a;
    if (m < b) m = b;
    if (m < c) m = c;
    return m;
}
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
    timer_init();

    PUT32(0x40040000, 640); /* #0 Physical Width */
    PUT32(0x40040004, 480); /* #4 Physical Height */
    PUT32(0x40040008, 640); /* #8 Virtual Width */
    PUT32(0x4004000C, 480); /* #12 Virtual Height */
    PUT32(0x40040010, 0); /* #16 GPU - Pitch */
    PUT32(0x40040014, 16); /* #20 Bit Depth */
    PUT32(0x40040018, 0); /* #24 X */
    PUT32(0x4004001C, 0); /* #28 Y */
    PUT32(0x40040020, 0); /* #32 GPU - Pointer */
    PUT32(0x40040024, 0); /* #36 GPU - Size */


    MailboxWrite(0x40040000,1);
    MailboxRead(1);
    rb=0x40040000;
    int i;
    for(i=0; i<640; i++){
    	multiplylookup[i] = i * 640;
    }

    
    ra=0;
    rb=GET32(0x40040020);
    //int x1, y1,x2,y2,color;
    //color = Random(53);
    drawLine(540,35,104,323,0b1111100000000000);
    //int offset, i;
    clrScreen(0x0000);
    uart_puts("timestamp: ");
    int timestamp;
    int color;
    color = Random(231452);
    //for(i = 0; i<10; i++){
    //while(1){
    timestamp = GetTimeStamp();
   	
    drawTriangle(300,0, 0, 479, 639, 479, 0xF000 );
   	
    //drawTriangle(x0,y0,x1,y1,x2,y2,color);
    
    hexstring(GetTimeStamp()-timestamp);
    
	//}
	
	uart_puts("\r\n");
	WaitMicros(5000);
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
