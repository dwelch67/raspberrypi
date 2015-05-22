
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define ARM_TIMER_LOD 0x3F00B400
#define ARM_TIMER_VAL 0x3F00B404
#define ARM_TIMER_CTL 0x3F00B408
#define ARM_TIMER_DIV 0x3F00B41C
#define ARM_TIMER_CNT 0x3F00B420

#define SYSTIMERCLO 0x3F003004
#define GPFSEL0     0x3F200000
#define GPFSEL1     0x3F200004
#define GPFSEL2     0x3F200008
#define GPSET0      0x3F20001C
#define GPCLR0      0x3F200028
#define GPPUD       0x3F200094
#define GPPUDCLK0   0x3F200098

#define TIMEOUT 1000000


//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    //for led

    //ra=GET32(GPFSEL1);
    //ra&=~(7<<18); //gpio16  OK LED
    //ra|=1<<18; //gpio16 output
    //PUT32(GPFSEL1,ra);

    //for jtag

    //alt4 = 0b011 3
    //alt5 = 0b010 2


    PUT32(GPPUD,0);
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<4)|(1<<22)|(1<<24)|(1<<25)|(1<<27));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);

    ra=GET32(GPFSEL0);
    ra&=~(7<<12); //gpio4
    ra|=2<<12; //gpio4 alt5 ARM_TDI
    PUT32(GPFSEL0,ra);

    ra=GET32(GPFSEL2);
    ra&=~(7<<6); //gpio22
    ra|=3<<6; //alt4 ARM_TRST
    ra&=~(7<<12); //gpio24
    ra|=3<<12; //alt4 ARM_TDO
    ra&=~(7<<15); //gpio25
    ra|=3<<15; //alt4 ARM_TCK
    ra&=~(7<<21); //gpio27
    ra|=3<<21; //alt4 ARM_TMS
    PUT32(GPFSEL2,ra);

//ARM_TRST      22 GPIO_GEN3 P1-15 IN  (22 ALT4)
//ARM_TDO     5/24 GPIO_GEN5 P1-18 OUT (24 ALT4)
//ARM_TCK    13/25 GPIO_GEN6 P1-22 OUT (25 ALT4)
//ARM_TDI     4/26 GPIO_GCLK P1-7   IN ( 4 ALT5)
//ARM_TMS    12/27 CAM_GPIO  S5-11 OUT (27 ALT4)

    PUT32(ARM_TIMER_CTL,0x00F90000);
    PUT32(ARM_TIMER_CTL,0x00F90200);

    rb=GET32(ARM_TIMER_CNT);
    while(1)
    {
        //PUT32(GPSET0,1<<16);
        while(1)
        {
            ra=GET32(ARM_TIMER_CNT);
            if((ra-rb)>=TIMEOUT) break;
        }
        rb+=TIMEOUT;
        //PUT32(GPCLR0,1<<16);
        while(1)
        {
            ra=GET32(ARM_TIMER_CNT);
            if((ra-rb)>=TIMEOUT) break;
        }
        rb+=TIMEOUT;
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
