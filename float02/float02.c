

//-------------------------------------------------------------------
//-------------------------------------------------------------------

#include "slowfloat.c"
#include "uart.h"

unsigned int myfun ( float x );

unsigned int m4add ( unsigned int, unsigned int );
unsigned int m4ftoi ( unsigned int );
unsigned int m4itof ( unsigned int );
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra,rb,rc;
    unsigned int fa,fb,fc;
    unsigned int xc;
    unsigned int beg,end;
    unsigned int errors;
    unsigned int cases;

    uart_init();
    init_timer();
    hexstring(0x12345678);
    hexstring(myfun(-2));
    hexstring(myfun(-4));
    hexstring(myfun(-8));
    hexstring(myfun(-5));
    hexstring(m4itof(0xFFFFFFFE));
    hexstring(m4itof(5));
    hexstring(m4ftoi(0xC0000000));
    hexstring(m4ftoi(0xC0800000));
    hexstring(m4ftoi(0xC1000000));



//enum {
    //float_round_nearest_even = 0,
    //float_round_down         = 1,
    //float_round_up           = 2,
    //float_round_to_zero      = 3
//};
    slow_float_rounding_mode = float_round_nearest_even;
//enum {
    //float_tininess_after_rounding  = 0,
    //float_tininess_before_rounding = 1
//};
    slow_float_detect_tininess = float_tininess_before_rounding;

    errors=0;
    cases=0;
    beg=timer_tick();
    for(ra=0;ra<int32NumP1;ra++)
    {
        for(rb=0;rb<int32NumP1;rb++)
        {
            slow_float_exception_flags = 0;
            fa=cases32[ra];
            fb=cases32[rb];
//if(((fa&0x7F800000)==0)&&(fa&0x007FFFFF))
//{
////    hexstrings(0xBAD00); hexstring(fa);
//continue;
//}
//if(((fb&0x7F800000)==0)&&(fb&0x007FFFFF))
//{
////    hexstrings(0xBAD01); hexstring(fb);
//continue;
//}
            fc=slow_float32_add(fa,fb);
//if(((fc&0x7F800000)==0)&&(fc&0x007FFFFF))
//{
    //hexstrings(0xBAD02); hexstring(fc);
//continue;
//}
if(fc==0xFFFFFFFF) continue; //is a nan thing
//hexstrings(fa); hexstring(fb);
            cases++;
            xc=m4add(fa,fb);
            if(fc!=xc)
            {
                hexstrings(slow_float_exception_flags);
                hexstrings(fa);
                hexstrings(fb);
                hexstrings(fc);
                hexstring(xc);
                errors++;
            }
            if(errors>20) break;
        }
        if(errors>20) break;
    }
    end=timer_tick();
    hexstring(end-beg);
    hexstring(cases);


    errors=0;
    cases=0;
    beg=timer_tick();
    for(ra=0;ra<int32NumP1;ra++)
    {
        slow_float_exception_flags = 0;
        fa=cases32[ra];
        rb=slow_float32_to_int32(fa);

if(slow_float_exception_flags&1) continue;

        cases++;
//hexstrings(fa); hexstring(rb);
        rc=m4ftoi(fa);
        if(rb!=rc)
        {
            hexstrings(slow_float_exception_flags);
            hexstrings(fa);
            hexstrings(rb);
            hexstring(rc);
            errors++;
        }
        if(errors>20) break;
    }
    end=timer_tick();
    hexstring(end-beg);
    hexstring(cases);




    hexstring(0x12345678);
    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------


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
