
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

#define ARMBASE 0x8000

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int GETPC ( void );
extern void BRANCHTO ( unsigned int );
extern void dummy ( unsigned int );

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

#define SOH 0x01
#define ACK 0x06
#define NAK 0x15
#define EOT 0x04
#define CAN 0x18

//timer set at 1 million ticks per second
#define NAKSOH_TIMEOUT 4000000
#define WAIT_AFTER_EOT_TIMEOUT 2000000
#define WAIT_AFTER_CAN_TIMEOUT 2000000

//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rx;
    unsigned int block;
    unsigned int state;
    unsigned int csum;
    unsigned char *xstring;

    uart_init();
    hexstring(0x12345678);
    hexstring(GETPC());
    hexstring(ARMBASE);
    timer_init();

    // block numbers start with 1

    // 132 byte packet
    // starts with SOH
    // block number byte
    // 255-block number
    // 128 bytes of data
    // checksum byte (whole packet)
    // a single EOT instead of SOH when done, send an ACK on it too

    while(1)
    {
        block=1;
        state=192;
        csum=0;
        xstring=(unsigned char *)ARMBASE;

        //only going to do the nak wait for soh dance once.
        //not going to deal with timeouts other than here
        uart_send(NAK);
        rx=timer_tick();
        while(1)
        {
            ra=timer_tick();
            if((ra-rx)>=NAKSOH_TIMEOUT)
            {
                uart_send(NAK);
                rx+=NAKSOH_TIMEOUT;
            }
            if(uart_check()) break;
        }
        while(state!=255)
        {
            rb=uart_recv();
            switch(state)
            {
                //wait for SOH or EOT
                case 192:
                {
                    switch(rb)
                    {
                        case SOH:
                        {
                            csum=rb;
                            state++;
                            break;
                        }
                        case EOT:
                        {
                            uart_send(ACK);
                            rx=timer_tick();
                            while(1)
                            {
                                ra=timer_tick();
                                if((ra-rx)>=WAIT_AFTER_EOT_TIMEOUT) break;
                            }
                            uart_send(0x0D);
                            uart_send(0x0A);
                            uart_send(0x0A);
                            hexstring(0xDADADADA);
                            while(1)
                            {
                                rb=uart_recv();
                                if((rb=='g')||(rb=='G'))
                                {
                                    uart_send(0x0D);
                                    uart_send('-');
                                    uart_send('-');
                                    uart_send(0x0D);
                                    uart_send(0x0A);
                                    uart_send(0x0A);
                                    BRANCHTO(ARMBASE);
                                }
                            }
                            break;
                        }
                        default:
                        {
                            state=255;
                            break;
                        }
                    }
                    break;
                }
                //uninverted block number
                case 193:
                {
                    if(rb==block)
                    {
                        csum+=rb;
                        state++;
                    }
                    else
                    {
                        state=255;
                    }
                    break;
                }
                //inverted block number
                case 194:
                {
                    if(rb==(0xFF-block))
                    {
                        csum+=rb;
                        state=0;
                    }
                    else
                    {
                        state=255;
                    }
                    break;
                }
                //checksum
                case 128:
                {
                    csum&=0xFF;
                    if(rb==csum)
                    {
                        uart_send(ACK);
                        block=(block+1)&0xFF;
                        xstring+=128;
                        state=192;
                    }
                    else
                    {
                        state=255;
                    }
                    break;
                }
                default:
                {
                    csum+=rb;
                    xstring[state]=rb&0xFF;
                    state++;
                    break;
                }
            }
        }
        //just bail out.
        uart_send(CAN);
        uart_send(CAN);
        uart_send(CAN);
        uart_send(CAN);
        uart_send(CAN);
        rx=timer_tick();
        while(1)
        {
            ra=timer_tick();
            if((ra-rx)>=WAIT_AFTER_CAN_TIMEOUT) break;
        }
        uart_send(0x0D);
        uart_send(0x0A);
        uart_send(0x0A);
        hexstring(0xBADBAD);
        uart_flush();
        //try to flush some out so we hit this timeout fewer times
        while(1)
        {
            if(uart_check())
            {
                uart_recv();
            }
            else
            {
                break;
            }
        }
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
