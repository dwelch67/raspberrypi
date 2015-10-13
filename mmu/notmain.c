
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

extern void start_mmu ( unsigned int, unsigned int );
extern void stop_mmu ( void );
extern void invalidate_tlbs ( void );
extern void invalidate_caches ( void );

extern void uart_init ( void );
extern void uart_send ( unsigned int );

extern void hexstrings ( unsigned int );
extern void hexstring ( unsigned int );

unsigned int system_timer_low ( void );

#define MMUTABLEBASE 0x00004000

//-------------------------------------------------------------------
unsigned int mmu_section ( unsigned int vadd, unsigned int padd, unsigned int flags )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    ra=vadd>>20;
    rb=MMUTABLEBASE|(ra<<2);
    rc=(padd&0xFFF00000)|0xC00|flags|2;
    //hexstrings(rb); hexstring(rc);
    PUT32(rb,rc);
    return(0);
}
//-------------------------------------------------------------------
unsigned int mmu_small ( unsigned int vadd, unsigned int padd, unsigned int flags, unsigned int mmubase )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    ra=vadd>>20;
    rb=MMUTABLEBASE|(ra<<2);
    rc=(mmubase&0xFFFFFC00)/*|(domain<<5)*/|1;
    //hexstrings(rb); hexstring(rc);
    PUT32(rb,rc); //first level descriptor
    ra=(vadd>>12)&0xFF;
    rb=(mmubase&0xFFFFFC00)|(ra<<2);
    rc=(padd&0xFFFFF000)|(0xFF0)|flags|2;
    //hexstrings(rb); hexstring(rc);
    PUT32(rb,rc); //second level descriptor
    return(0);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    uart_init();
    hexstring(0xDEADBEEF);

    PUT32(0x00045678,0x00045678);
    PUT32(0x00145678,0x00145678);
    PUT32(0x00245678,0x00245678);
    PUT32(0x00345678,0x00345678);

    PUT32(0x00346678,0x00346678);
    PUT32(0x00146678,0x00146678);

    PUT32(0x0AA45678,0x12345678);
    PUT32(0x0BB45678,0x12345678);
    PUT32(0x0CC45678,0x12345678);
    PUT32(0x0DD45678,0x12345678);

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

    for(ra=0;;ra+=0x00100000)
    {
        mmu_section(ra,ra,0x0000);
        if(ra==0xFFF00000) break;
    }

    //mmu_section(0x00000000,0x00000000,0x0000|8|4);
    //mmu_section(0x00100000,0x00100000,0x0000);
    //mmu_section(0x00200000,0x00200000,0x0000);
    //mmu_section(0x00300000,0x00300000,0x0000);
    //peripherals
    mmu_section(0x20000000,0x20000000,0x0000); //NOT CACHED!
    mmu_section(0x20200000,0x20200000,0x0000); //NOT CACHED!

    start_mmu(MMUTABLEBASE,0x00000001|0x1000|0x0004); //[23]=0 subpages enabled = legacy ARMv4,v5 and v6

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

    mmu_section(0x00100000,0x00300000,0x0000);
    mmu_section(0x00200000,0x00000000,0x0000);
    mmu_section(0x00300000,0x00100000,0x0000);
    invalidate_tlbs();

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

    for(ra=0;ra<4;ra++)
    {
        hexstring(system_timer_low());
    }
    uart_send(0x0D); uart_send(0x0A);

    mmu_section(0x20000000,0x20000000,0x0000|8); //CACHED
    invalidate_tlbs();

    for(ra=0;ra<4;ra++)
    {
        hexstring(system_timer_low());
    }
    uart_send(0x0D); uart_send(0x0A);

    mmu_small(0x0AA45000,0x00145000,0,0x00000400);
    mmu_small(0x0BB45000,0x00245000,0,0x00000800);
    mmu_small(0x0CC45000,0x00345000,0,0x00000C00);
    mmu_small(0x0DD45000,0x00345000,0,0x00001000);
    mmu_small(0x0DD46000,0x00146000,0,0x00001000);
    mmu_small(0x0DD03000,0x20003000,0,0x00001000);
    mmu_section(0x00300000,0x00300000,0x0000);
    invalidate_tlbs();


    hexstring(GET32(0x0AA45678));
    hexstring(GET32(0x0BB45678));
    hexstring(GET32(0x0CC45678));
    uart_send(0x0D); uart_send(0x0A);


    hexstring(GET32(0x00345678));
    hexstring(GET32(0x00346678));
    hexstring(GET32(0x0DD45678));
    hexstring(GET32(0x0DD46678));
    uart_send(0x0D); uart_send(0x0A);

    for(ra=0;ra<4;ra++)
    {
        hexstring(GET32(0x0DD03004));
    }
    uart_send(0x0D); uart_send(0x0A);


    //access violation.

    mmu_section(0x00100000,0x00100000,0x0020);
    invalidate_tlbs();

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

    hexstring(0xDEADBEEF);

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
