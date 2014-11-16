
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
extern void uart_init ( void );
extern void hexstring ( unsigned int d );
extern void hexstrings ( unsigned int d );
extern void start_l1cache ( void );
extern void stop_l1cache ( void );
extern void start_mmu ( unsigned int, unsigned int );
extern unsigned int LDREX ( unsigned int, unsigned int );
extern unsigned int STREX ( unsigned int, unsigned int, unsigned int );
extern unsigned int EXTEST ( unsigned int, unsigned int, unsigned int );

#define MMUTABLEBASE 0x00100000

//-------------------------------------------------------------------
unsigned int mmu_section ( unsigned int add, unsigned int flags )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    ra=add>>20;
    rb=MMUTABLEBASE|(ra<<2);
    rc=(ra<<20)|flags|2;
    PUT32(rb,rc);
    return(0);
}
//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra,rb,rc;

    uart_init();

    for(ra=0;ra<0x20;ra++) hexstring(ra);

    hexstring(0x12345678);

    //ram used by the stack and the program
    if(mmu_section(0x00000000,0x0000|8|4)) return(1);
    //Memory mapped I/O used by the uart, etc, not cached
    if(mmu_section(0x20000000,0x0000)) return(1);
    if(mmu_section(0x20100000,0x0000)) return(1);
    if(mmu_section(0x20200000,0x0000)) return(1);

    //not enabling data cache just yet.
    start_mmu(MMUTABLEBASE,0x00800001);
    hexstring(0x12345678);

    PUT32(0x00009000,0x1234);
    ra=LDREX(0,0x00009000);
    rb=STREX(0,0xABCD,0x00009000);
    rc=EXTEST(0,0x1234,0x00009000);
    hexstrings(ra); hexstrings(rb); hexstring(rc);

    ra=LDREX(0,0x00009000);
    rb=STREX(0,0xABCD,0x00009000);
    rc=EXTEST(0,0x1234,0x00009000);
    hexstrings(ra); hexstrings(rb); hexstring(rc);

    start_l1cache();
    GET32(0x00009000);
    ra=LDREX(0,0x00009000);
    rb=STREX(0,0x5678,0x00009000);
    rc=EXTEST(0,0x2345,0x00009000);
    stop_l1cache();
    hexstrings(ra); hexstrings(rb); hexstring(rc);

    ra=LDREX(0,0x00009000);
    rb=STREX(0,0x1234,0x00009000);
    rc=EXTEST(0,0x3456,0x00009000);
    hexstrings(ra); hexstrings(rb); hexstring(rc);

    hexstring(0x12345678);
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
