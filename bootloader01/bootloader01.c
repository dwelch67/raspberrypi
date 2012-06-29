
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void BRANCHTO ( unsigned int );
extern void dummy ( unsigned int );

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x01) break;
    }
    return(GET32(AUX_MU_IO_REG)&0xFF);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int c )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;
    }
    PUT32(AUX_MU_IO_REG,c);
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------
// Packet Format
// [0] 0x7C  start sync
// [1] datalen
// [2] ~datalen
// datalen bytes:
// [3] type
// [4] sequence
// [5] pass=0/fail!=0
// [6]
// [7]
// [8] payload starts
// ...
// datalen ends
// [n-2] 0x7D end sync
// [n-1] checksum
//------------------------------------------------------------------------
unsigned char xstring[512];
//------------------------------------------------------------------------
int recv_packet ( void )
{
    unsigned int len;
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    while(1)
    {
        ra=uart_recv();
        if(ra!=0x7C)
        {
            continue;
        }
        ra=0;
        xstring[ra++]=0x7C; //sync
        xstring[ra++]=uart_recv(); //datalen
        xstring[ra++]=uart_recv(); //~datalen
        if(xstring[ra-2]!=(~xstring[ra-1]&0xFF))
        {
            continue;
        }
        len=xstring[ra-2];
        while(len--) xstring[ra++]=uart_recv();
        xstring[ra++]=uart_recv(); //sync
        xstring[ra++]=uart_recv(); //checksum
        if(xstring[ra-2]!=0x7D)
        {
            continue;
        }
        for(rb=0,rc=0x00;rb<ra;rb++) rc+=xstring[rb];
        rc&=0xFF;
        if(rc==0xFF) break;
    }
    return(0);
}
//------------------------------------------------------------------------
int send_result ( unsigned char res )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    ra=0;
    xstring[ra++]=0x7C; //xstring[ra++]=0x7C; //sync
    xstring[ra++]=0x05; //xstring[ra++]=uart_recv(); //datalen
    xstring[ra++]=~0x05;//xstring[ra++]=uart_recv(); //~datalen
    //datalen:
    ra++; //same as input
    ra++; //same as input
    xstring[ra++]=res;
    xstring[ra++]=0;
    xstring[ra++]=0;
    //datalen
    xstring[ra++]=0x7D;
    for(rb=0,rc=0;rb<ra;rb++) rc+=xstring[rb];
    xstring[ra++]=(~rc)&0xFF;
    for(rb=0;rb<ra;rb++) uart_send(xstring[rb]);
    return(0);
}

//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int len;
    unsigned int addr;
    unsigned int data;

    PUT32(AUX_ENABLES,1);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,270);

    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    PUT32(GPFSEL1,ra);

    PUT32(GPPUD,0);
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);

    PUT32(AUX_MU_CNTL_REG,3);

    hexstring(0x12345678);

    while(1)
    {
        if(recv_packet()) break;
        switch(xstring[3])
        {
            case 0x07: //write to ram
            {
                //validate len
                len=xstring[1];
                if(len<(5+4+2))
                {
                    send_result(1);
                    break;
                }
                if((len-5-4-2)&1)
                {
                    send_result(1);
                    break;
                }
                //[5,6,7,8,9] packet header
                //[8,9,10,11] address
                //[12,13] first halfword
                //[14,15] second halfword
                ra=8;
                addr=0;
                addr<<=8; addr|=xstring[ra++];
                addr<<=8; addr|=xstring[ra++];
                addr<<=8; addr|=xstring[ra++];
                addr<<=8; addr|=xstring[ra++];
                len-=5+4;
                while(len)
                {
                    data=0;
                    data<<=8; data|=xstring[ra++];
                    data<<=8; data|=xstring[ra++];
                    PUT16(addr,data);
                    addr+=2;
                    len-=2;
                }
                send_result(0);
                break;
            }
            case 0x08: //branch to address
            {
                //validate len
                len=xstring[1];
                if(len!=(5+4))
                {
                    send_result(1);
                    break;
                }
                //[5,6,7,8,9] packet header
                //[8,9,10,11] address
                ra=8;
                addr=0;
                addr<<=8; addr|=xstring[ra++];
                addr<<=8; addr|=xstring[ra++];
                addr<<=8; addr|=xstring[ra++];
                addr<<=8; addr|=xstring[ra++];
                send_result(0);
                BRANCHTO(addr);
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
