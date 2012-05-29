
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#include "testdata.h"
#include "zlib.h"

#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_CNT 0x2000B420

unsigned int heap_offset;
unsigned int complen;
unsigned int uncomplen;
unsigned char compressed[TESTDATALEN];
unsigned char uncompressed[TESTDATALEN];

//-------------------------------------------------------------------------
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void * mkptr ( unsigned int );
//------------------------------------------------------------------------
void uart_init ( void );
void hexstring ( unsigned int d );
void hexstrings ( unsigned int d );
//------------------------------------------------------------------------
unsigned char *ZMEMCPY(unsigned char *dest, const unsigned char *src, unsigned int n)
{
    unsigned int ra;
    for(ra=0;ra<n;ra++) dest[ra]=src[ra];
    return(dest);
}
//------------------------------------------------------------------------
//       void *memset(void *s, int c, size_t n);
unsigned char *ZMEMSET( unsigned char *s, int c, unsigned int n )
{
    unsigned int ra;
    for(ra=0;ra<n;ra++) s[ra]=(unsigned char)c;
    return(s);
}
//------------------------------------------------------------------------
//       void *malloc(size_t size);
void *ZMALLOC ( unsigned int size )
{
    void *ret;

//    hexstring(heap_offset);
    ret=(void *)heap_offset;
    heap_offset+=size;
    heap_offset=(heap_offset+3)&(~3);
    return(ret);
}
//------------------------------------------------------------------------
void *ZCALLOC(unsigned int nmemb, unsigned int size)
{
    unsigned char *ret;

    ret=ZMALLOC(nmemb*size);
    ZMEMSET(ret,0,nmemb*size);
    return(ret);
}
//------------------------------------------------------------------------
void free(void *ptr)
{
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int beg,mid,end;
    int ret1,ret2;

    heap_offset=0x100000;

    uart_init();

    PUT32(ARM_TIMER_CTL,0x00F90000);
    PUT32(ARM_TIMER_CTL,0x00F90200);

    for(ra=0;ra<0x100;ra++) hexstring(ra);
    hexstring(TESTDATALEN);


    beg=GET32(ARM_TIMER_CNT);
    complen=TESTDATALEN;
    ret1=compress(compressed,&complen,testdata,TESTDATALEN);
    mid=GET32(ARM_TIMER_CNT);
    uncomplen=TESTDATALEN;
    ret2=uncompress(uncompressed,&uncomplen,compressed,complen);
    end=GET32(ARM_TIMER_CNT);


    hexstring(ret1);
    hexstring(complen);
    hexstring(ret2);
    hexstring(uncomplen);

    hexstring(0x11111111);
    hexstring(beg);
    hexstring(mid);
    hexstring(end);
    hexstring(mid-beg);
    hexstring(end-mid);
    hexstring(end-beg);




    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
