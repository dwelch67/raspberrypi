
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void HOP ( unsigned int );

//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    ra=0x10000;

    PUT32(ra,0xe3a0043f); ra+=4;
    PUT32(ra,0xe3800602); ra+=4;
    PUT32(ra,0xe5901010); ra+=4;
    PUT32(ra,0xe3c1160e); ra+=4;
    PUT32(ra,0xe3811602); ra+=4;
    PUT32(ra,0xe5801010); ra+=4;
    PUT32(ra,0xe590100c); ra+=4;
    PUT32(ra,0xe3c1190e); ra+=4;
    PUT32(ra,0xe3811902); ra+=4;
    PUT32(ra,0xe580100c); ra+=4;
    PUT32(ra,0xe3a02902); ra+=4;
    PUT32(ra,0xe3a03008); ra+=4;
    PUT32(ra,0xe5802020); ra+=4;
    PUT32(ra,0xe580302c); ra+=4;
    PUT32(ra,0xe3a04601); ra+=4;
    PUT32(ra,0xe2544001); ra+=4;
    PUT32(ra,0x1afffffd); ra+=4;
    PUT32(ra,0xe580202c); ra+=4;
    PUT32(ra,0xe5803020); ra+=4;
    PUT32(ra,0xe3a04601); ra+=4;
    PUT32(ra,0xe2544001); ra+=4;
    PUT32(ra,0x1afffffd); ra+=4;
    PUT32(ra,0xeafffff4); ra+=4;

    HOP(0x10000);
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
