

//-------------------------------------------------------------------
//-------------------------------------------------------------------

#include "uart.h"

void PUT32 ( unsigned int, unsigned int );
unsigned int m4add ( unsigned int, unsigned int, unsigned int, unsigned int );
unsigned int m4add4 ( unsigned int, unsigned int, unsigned int, unsigned int );
void m4vmov ( unsigned int );

//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    rb=0xEA000000+((0x8000>>2)-2);
    for(ra=0x00;ra<0x40;ra+=4) PUT32(ra,rb);
    uart_init();
    hexstring(0x12345678);
    m4vmov(0xABCDABCD);
    hexstring(m4add(0x3F800000,0x00000000,0x3F801230,0x3F801110));
    m4vmov(0xABCDABCD);
    hexstring(m4add(0x00012345,0x00000111,0x3F801230,0x3F802220));
    m4vmov(0xABCDABCD);
    hexstring(m4add(0x00000000,0x00012345,0x3F801230,0x3F803330));
    m4vmov(0xABCDABCD);
    hexstring(m4add(0x3F801230,0x3F801110,0x00000111,0x00012345));
    m4vmov(0xABCDABCD);
    hexstring(m4add(0x3F800000,0x00000000,0x3F801230,0x3F804440));
    hexstring(0x22222222);
    m4vmov(0xABCDABCD);
    hexstring(m4add2(0x3F801230,0x3F805550,0x3F800000,0x00000000));
    m4vmov(0xABCDABCD);
    hexstring(m4add2(0x00000111,0x00012345,0x3F801230,0x3F806660));
    m4vmov(0xABCDABCD);
    hexstring(m4add2(0x3F801230,0x3F807770,0x00000111,0x00012345));
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
