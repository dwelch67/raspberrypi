
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void uart_init ( void );
extern void hexstrings ( unsigned int d );
extern void hexstring ( unsigned int d );
extern void init_timer ( void );
extern unsigned int timer_tick ( void );
extern void start_l1cache ( void );
extern void stop_l1cache ( void );
extern void ARMTEST0 ( unsigned int );
extern void ARMTEST1 ( unsigned int );
extern void ARMTEST2 ( unsigned int );
extern void ARMTEST3 ( unsigned int );
extern void THUMBTEST0 ( unsigned int );
extern void THUMBTEST1 ( unsigned int );
extern void THUMBTEST2 ( unsigned int );

//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int beg,end;

    uart_init();
    hexstring(0x12345678);

    start_l1cache();

    init_timer();
    beg=timer_tick();
    ARMTEST0(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    ARMTEST0(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    ARMTEST1(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    ARMTEST2(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    ARMTEST3(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    THUMBTEST0(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    THUMBTEST1(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    THUMBTEST2(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    stop_l1cache();

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
