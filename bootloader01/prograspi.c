
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2000, 2003, 2008, 2009, 2012
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

#include "ser.h"

unsigned int seq;
unsigned int ra,rb,rc,rd;
unsigned int addr;
unsigned int firstaddr;

unsigned char data[128];
unsigned char sdata[512];
unsigned char rdata[5000];

//-----------------------------------------------------------------------------
int check_packet ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;


    if(rdata[0]!=0x7C) return(2);
    if(rdata[1]!=(~rdata[2]&0xFF)) return(2);
    if(rdata[3]!=sdata[3]) return(2);
    if(rdata[4]!=sdata[4]) return(2);
    if(rdata[3+rdata[1]]!=0x7D) return(2);
    ra=3;
    ra=rdata[1]+5;
    for(rb=0,rc=0;rb<ra;rb++) rc+=rdata[rb];
    rc&=0xFF;
    if(rc!=0xFF) return(2);
    return(rdata[5]);
}
//-----------------------------------------------------------------------------
int raspload ( unsigned int addr, unsigned int data )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;

    //printf("load 0x%08X, 0x%04X\n",addr,data);

    if(firstaddr==0xFFFFFFFF) firstaddr=addr;

    ra=0;
    sdata[ra++]=0x7C;
    sdata[ra++]=2+5+4;
    sdata[ra]=~sdata[ra-1];  ra++;
    //
    sdata[ra++]=0x07;
    sdata[ra++]=seq&0xFF; seq++;
    sdata[ra++]=0;
    sdata[ra++]=0;
    sdata[ra++]=0;

    sdata[ra++]=(addr>>24)&0xFF;
    sdata[ra++]=(addr>>16)&0xFF;
    sdata[ra++]=(addr>> 8)&0xFF;
    sdata[ra++]=(addr>> 0)&0xFF;
//    for(rd=0;rd<rb;rd+=2)
    {
        sdata[ra++]=(data>>8)&0xFF;
        sdata[ra++]=(data>>0)&0xFF;
    }
    //
    sdata[ra++]=0x7D;
    for(rd=0,rc=0;rd<ra;rd++) rc+=sdata[rd];
    sdata[ra++]=(~rc)&0xFF;

    //for(rd=0;rd<ra;rd++) printf("%02X ",sdata[rd]); printf("\n");

    //for(rd=0,rc=0x00;rd<ra;rd++) rc+=sdata[rd];
    //rc&=0xFF;

    ser_senddata(sdata,ra);
    rb=0;
    rc=0;
    while(1)
    {
        rb=ser_copystring(rdata);
        //if(rb!=rc) printf("%u\n",rb);
        rc=rb;
        if(rb==10)
        {
            //for(ra=0;ra<rb;ra++) printf("%02X ",rdata[ra]); printf("\n");
            ra=check_packet();
            if(ra) return(1);
            ser_dump(rc);
            break;
        }
    }
    return(0);
}
//-----------------------------------------------------------------------------
int readhex ( FILE *fp )
{
    char gstring[80];
    char newline[1024];
    unsigned char hexline[1024];

    //intimately tied to the .balign value in the bootloader startup
    #define RAMMASK 0x1FFFFF

    unsigned int addhigh;
    unsigned int add;
    unsigned int data;

    unsigned int ra;

    unsigned int line;

    unsigned char checksum;

    unsigned int len;
    unsigned int maxadd;

    unsigned char t;

    maxadd=0;

    addhigh=0;

    line=0;
    while(fgets(newline,sizeof(newline)-1,fp))
    {
        line++;
        //printf("%s",newline);
        if(newline[0]!=':')
        {
            printf("Syntax error <%u> no colon\n",line);
            continue;
        }
        gstring[0]=newline[1];
        gstring[1]=newline[2];
        gstring[2]=0;
        len=strtoul(gstring,NULL,16);
        for(ra=0;ra<(len+5);ra++)
        {
            gstring[0]=newline[(ra<<1)+1];
            gstring[1]=newline[(ra<<1)+2];
            gstring[2]=0;
            hexline[ra]=(unsigned char)strtoul(gstring,NULL,16);
        }
        checksum=0;
        for(ra=0;ra<(len+5);ra++) checksum+=hexline[ra];
        //checksum&=0xFF;
        if(checksum)
        {
            printf("checksum error <%u>\n",line);
        }
        add=hexline[1]; add<<=8;
        add|=hexline[2];
        add|=addhigh;
        if(add>RAMMASK)
        {
            printf("address too big 0x%08X\n",add);
            //return(1);
            continue;
        }
        if(add&3)
        {
            printf("bad address 0x%08X\n",add);
            return(1);
        }
        t=hexline[3];
        if(t!=0x02)
        {
            if(len&3)
            {
                printf("bad length\n");
                return(1);
            }
        }

        //:0011223344
        //;llaaaattdddddd
        //01234567890
        switch(t)
        {
            default:
                printf("UNKNOWN type %02X <%u>\n",t,line);
                break;
            case 0x00:
                for(ra=0;ra<len;ra+=4)
                {
                    if(add>RAMMASK)
                    {
                        printf("address too big 0x%08X\n",add);
                        break;
                    }
                    data=           hexline[ra+4+1];
                    data<<=8; data|=hexline[ra+4+0];
                    if(raspload(add,data)) return(1);
                    add+=2;

                    data=           hexline[ra+4+3];
                    data<<=8; data|=hexline[ra+4+2];
                    if(raspload(add,data)) return(1);
                    add+=2;
                    if(add>maxadd) maxadd=add;
                }
                break;
            case 0x01:
//                printf("End of data\n");
                break;
            case 0x02:
                addhigh=hexline[5];
                addhigh<<=8;
                addhigh|=hexline[4];
                addhigh<<=16;
                break;
            case 0x03:
                    data=           hexline[4+0];
                    data<<=8; data|=hexline[4+1];
                    data<<=8; data|=hexline[4+2];
                    data<<=8; data|=hexline[4+3];
                    printf("TYPE 3 0x%08X\n",data);
                break;
        }
    }

    //printf("%u lines processed\n",line);
    //printf("%08X\n",maxadd);
    //for(ra=0;ra<maxadd;ra+=4)
    //{
        //printf("0x%08X: 0x%08X\n",ra,ram[ra>>2]);
    //}
    return(0);


}




//-----------------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    unsigned int ra;
    //unsigned int rb;
    unsigned int rc;

    if(argc<3)
    {
        printf("prograspi filename.hex /dev/ttyXYZ\n");
        return(1);
    }
    fp=fopen(argv[1],"rt");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    printf("file opened\n");


    if(ser_open(argv[2]))
    {
        printf("ser_open() failed\n");
        return(1);
    }
    printf("port opened\n");

    seq=17;
    firstaddr=0xFFFFFFFF;
    if(readhex(fp))
    {
        return(1);
    }

    printf("Entry point: 0x%08X\n",firstaddr);

    ra=0;
    sdata[ra++]=0x7C;
    sdata[ra++]=5+4;
    sdata[ra]=~sdata[ra-1]; ra++;
    //
    sdata[ra++]=0x08;
    sdata[ra++]=seq&0xFF; seq++;
    sdata[ra++]=0;
    sdata[ra++]=0;
    sdata[ra++]=0;

    sdata[ra++]=(firstaddr>>24)&0xFF;
    sdata[ra++]=(firstaddr>>16)&0xFF;
    sdata[ra++]=(firstaddr>> 8)&0xFF;
    sdata[ra++]=(firstaddr>> 0)&0xFF;
    //
    sdata[ra++]=0x7D;
    for(rd=0,rc=0;rd<ra;rd++) rc+=sdata[rd];
    sdata[ra++]=(~rc)&0xFF;

    //for(rd=0;rd<ra;rd++) printf("%02X ",sdata[rd]); printf("\n");

    for(rd=0,rc=0x00;rd<ra;rd++) rc+=sdata[rd];
    rc&=0xFF;

    ser_senddata(sdata,ra);

    ser_close();
    printf("\n\n");
    return(0);
}
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2000, 2003, 2008, 2009, 2012
//-----------------------------------------------------------------------------


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

