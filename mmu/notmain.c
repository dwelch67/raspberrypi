
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

extern void start_mmu ( unsigned int, unsigned int );
extern void stop_mmu ( void );
extern void invalidate_tlbs ( void );

extern void uart_init ( void );
extern void uart_send ( unsigned int );

extern void hexstrings ( unsigned int );
extern void hexstring ( unsigned int );

#define MMUTABLEBASE 0x00004000

//-------------------------------------------------------------------
unsigned int mmu_section ( unsigned int vadd, unsigned int padd, unsigned int flags )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    ra=vadd>>20;
    rb=MMUTABLEBASE|(ra<<2);
    ra=padd>>20;
    rc=(ra<<20)|flags|2;
    PUT32(rb,rc);
    return(0);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    uart_init();
    hexstring(0xDEADBEEF);

    PUT32(0x00045678,0x00045678);
    PUT32(0x00145678,0x00145678);
    PUT32(0x00245678,0x00245678);
    PUT32(0x00345678,0x00345678);

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

    mmu_section(0x00000000,0x00000000,0x0000|8|4);
    mmu_section(0x00100000,0x00100000,0x0000);
    mmu_section(0x00200000,0x00200000,0x0000);
    mmu_section(0x00300000,0x00300000,0x0000);
    //peripherals
    mmu_section(0x20000000,0x20000000,0x0000); //NOT CACHED!
    mmu_section(0x20200000,0x20200000,0x0000); //NOT CACHED!

    start_mmu(MMUTABLEBASE,0x00800001|0x1000|0x0004);
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

    mmu_section(0x00100000,0x00100000,0x0020);

    invalidate_tlbs();
    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

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
