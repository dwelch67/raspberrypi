
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// 2  outer corner
// 4
// 6
// 8  TX out
// 10 RX in

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

//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    uart_init();
    hexstring(0x12345678);
    hexstring(GETPC());

    //e12fff1e    bx  lr
    //ee100f10    mrc 15, 0, r0, cr0, cr0, {0}
    //ee100f30    mrc 15, 0, r0, cr0, cr0, {1}
    //ee100f50    mrc 15, 0, r0, cr0, cr0, {2}
    //ee100f70    mrc 15, 0, r0, cr0, cr0, {3}
    //ee100f90    mrc 15, 0, r0, cr0, cr0, {4}
    //ee100fb0    mrc 15, 0, r0, cr0, cr0, {5}
    //ee100fd0    mrc 15, 0, r0, cr0, cr0, {6}
    //ee100ff0    mrc 15, 0, r0, cr0, cr0, {7}
    //ee100f11    mrc 15, 0, r0, cr0, cr1, {0}
    //ee100f31    mrc 15, 0, r0, cr0, cr1, {1}
    //ee100f51    mrc 15, 0, r0, cr0, cr1, {2}
    //ee100f71    mrc 15, 0, r0, cr0, cr1, {3}
    //ee100f91    mrc 15, 0, r0, cr0, cr1, {4}
    //ee100fb1    mrc 15, 0, r0, cr0, cr1, {5}
    //ee100fd1    mrc 15, 0, r0, cr0, cr1, {6}
    //ee100ff1    mrc 15, 0, r0, cr0, cr1, {7}
    //ee100f12    mrc 15, 0, r0, cr0, cr2, {0}
    //ee100f32    mrc 15, 0, r0, cr0, cr2, {1}
    //ee100f52    mrc 15, 0, r0, cr0, cr2, {2}
    //ee100f72    mrc 15, 0, r0, cr0, cr2, {3}
    //ee100f92    mrc 15, 0, r0, cr0, cr2, {4}
    //ee100fb2    mrc 15, 0, r0, cr0, cr2, {5}
    //ee100fd2    mrc 15, 0, r0, cr0, cr2, {6}
    //ee100ff2    mrc 15, 0, r0, cr0, cr2, {7}
    for(ra=0;ra<8;ra++)
    {
        PUT32(0x4000,0xee100f10|(ra<<5));
        PUT32(0x4004,0xe12fff1e);
        hexstrings(ra);
        hexstring(BRANCHTO(0x4000));
    }
    for(ra=0;ra<8;ra++)
    {
        PUT32(0x4000,0xee100f11|(ra<<5));
        PUT32(0x4004,0xe12fff1e);
        hexstrings(ra);
        hexstring(BRANCHTO(0x4000));
    }
    for(ra=0;ra<8;ra++)
    {
        PUT32(0x4000,0xee100f12|(ra<<5));
        PUT32(0x4004,0xe12fff1e);
        hexstrings(ra);
        hexstring(BRANCHTO(0x4000));
    }
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
