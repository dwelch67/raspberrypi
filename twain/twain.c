
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#define ICACHE
#define MMU
#define DCACHE

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
extern void start_mmu ( unsigned int, unsigned int );

#include "zlib.h"
#include "twain.h"
#include "twaindef.h"

unsigned char cdata[TESTDATALEN];
unsigned char udata[TESTDATALEN];

unsigned int malloc_base;

//program 0x00000000 to 0x00200000
//   0x00030000 - 0x00080000 heap
//0x00200000 mmu table


//unsigned int copycount;
//unsigned int setcount;


#define MALLOCBASE   0x00030000

#define MMUTABLEBASE 0x00200000

#define MMUTABLESIZE (0x8000)
#define MMUTABLEMASK ((MMUTABLESIZE-1)>>2)

#define TOP_LEVEL_WORDS (1<<((31-20)+1))
#define COARSE_TABLE_WORDS (1<<((19-12)+1))
#define SMALL_TABLE_WORDS (1<<((11-0)+1))

unsigned int nextfree;

//-------------------------------------------------------------------
unsigned int next_coarse_offset ( unsigned int x )
{
    unsigned int mask;

    mask=(~0)<<(10-2);
    mask=~mask;
    while(x&mask) x++; //lazy brute force
    return(x);
}
//-------------------------------------------------------------------
unsigned int add_one ( unsigned int add, unsigned int flags )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    //bits 31:20 index into the top level table
    ra=add>>20;
    rc=MMUTABLEBASE+(ra<<2);
    rb=GET32(rc);
    if(rb)
    {
        //printf("Address %08X already allocated\n",add);
        hexstring(add);
        hexstring(rc);
        hexstring(rb);
        hexstring(0xBADADD);
        return(1);
    }
    add=ra<<20;

    rb=next_coarse_offset(nextfree);
    rc=rb+COARSE_TABLE_WORDS;
    if(rc>=MMUTABLESIZE)
    {
        //printf("Not enough room\n");
        hexstring(0xBAD);
        return(1);
    }
    nextfree=rc;
    //use course page table pointer on top level table
    PUT32(MMUTABLEBASE+(ra<<2),(MMUTABLEBASE+(rb<<2))|0x00000001);
    //fill in the course page table. with small entries
    for(ra=0;ra<COARSE_TABLE_WORDS;ra++)
    {
        PUT32(MMUTABLEBASE+(rb<<2)+(ra<<2),(add+(ra<<12))|0x00000032|flags);
    }
    return(0);
}
//------------------------------------------------------------------------
void * xmemcpy (void *dest, const void *src, unsigned int len)
{
  char *d = dest;
  const char *s = src;
    //copycount++;
    //hexstrings((unsigned int)dest);
    //hexstrings((unsigned int)src);
    //hexstring((unsigned int)len);
  while (len--)
    *d++ = *s++;
  return dest;
}

//------------------------------------------------------------------------
void * xmemset (void *dest, int val, unsigned int len)
{
  unsigned char *ptr = dest;
    //setcount++;
    //hexstrings((unsigned int)dest);
    //hexstrings((unsigned int)src);
    //hexstring((unsigned int)len);
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}
//------------------------------------------------------------------------
void * xmalloc ( unsigned int size )
{
    void *ret;

    ret=(void *)malloc_base;
    malloc_base+=size;
    malloc_base+=3;
    malloc_base&=~3;
    return(ret);
}
//------------------------------------------------------------------------
void * xcalloc ( unsigned int a, unsigned int b )
{
    void *ret;

    ret=xmalloc(a*b);
    xmemset(ret,0,a*b);
    return(ret);
}
//------------------------------------------------------------------------
void xfree ( void *ptr )
{
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned long cdest;
    unsigned long udest;
    int cret;
    int uret;
    unsigned int cerrors;
    unsigned int uerrors;
    unsigned int ra;
    unsigned int beg,end;

#ifdef ICACHE
    start_l1cache();
#endif

    //copycount=0;
    //setcount=0;

    uart_init();
    hexstring(0x12345678);

#ifdef MMU
    for(nextfree=0;nextfree<TOP_LEVEL_WORDS;nextfree++) PUT32(MMUTABLEBASE+(nextfree<<2),0);
    //nextfree=TOP_LEVEL_WORDS;

    //ram used by the stack and the program
    if(add_one(0x00000000,0x0000|8|4)) return(1);
    if(add_one(0x00100000,0x0000|8|4)) return(1);
    if(add_one(0x00200000,0x0000|8|4)) return(1);
    //Memory mapped I/O used by the uart, etc, not cached
    if(add_one(0x20000000,0x0000)) return(1);
    if(add_one(0x20100000,0x0000)) return(1);
    if(add_one(0x20200000,0x0000)) return(1);

#ifdef DCACHE
    start_mmu(MMUTABLEBASE,0x00800005);
#else
    start_mmu(MMUTABLEBASE,0x00800001);
#endif
#endif

    init_timer();

    malloc_base=MALLOCBASE;

    beg=timer_tick();
    cdest=sizeof(cdata);
    cret=compress(cdata,&cdest,twain,TESTDATALEN);
    udest=sizeof(udata);
    uret=uncompress(udata,&udest,twaindef,TESTDATALENDEF);

    end=timer_tick();

    hexstring(0x1111);
    hexstring(malloc_base);
    hexstring(cret);
    hexstring(cdest);
    hexstring(uret);
    hexstring(udest);
    hexstring(0x2222);
    hexstrings(beg); hexstrings(end); hexstring(end-beg);
    hexstring(0x3333);
    //hexstring(setcount);
    //hexstring(copycount);
    //hexstring(0x4444);
    hexstrings(cdest); hexstring(TESTDATALENDEF);
    hexstrings(udest); hexstring(TESTDATALEN);
    cerrors=0;
    for(ra=0;ra<TESTDATALENDEF;ra++)
    {
        if(cdata[ra]!=twaindef[ra])
        {
            cerrors++;
        }
    }
    uerrors=0;
    for(ra=0;ra<TESTDATALEN;ra++)
    {
        if(udata[ra]!=twain[ra])
        {
            uerrors++;
        }
    }
    if(cerrors)
    {
        hexstrings(cerrors); hexstring(0xBADBAD);
    }
    else
    {
        hexstring(cerrors);
    }
    if(uerrors)
    {
        hexstrings(uerrors); hexstring(0xBADBAD);
    }
    else
    {
        hexstring(uerrors);
    }

    hexstring(0x12345678);

#ifdef ICACHE
    stop_l1cache();
#endif

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
