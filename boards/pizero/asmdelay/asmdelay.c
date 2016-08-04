
//-------------------------------------------------------------------
// Copyright (C) 2010 Netronome Systems
//-------------------------------------------------------------------

//d6004024 <ASMDELAY>:
//d6004024:	e2500001 	subs	r0, r0, #1
//d6004028:	1afffffd 	bne	d6004024 <ASMDELAY>
//d600402c:	e12fff1e 	bx	lr

#define ARM_TIMER_LOD 0x2000B400
#define ARM_TIMER_VAL 0x2000B404
#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_DIV 0x2000B41C
#define ARM_TIMER_CNT 0x2000B420


extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void ASMDELAY ( unsigned int );
extern void uart_init(void);
extern void hexstrings ( unsigned int );
extern void hexstring ( unsigned int );
extern void HOP ( unsigned int, unsigned int );

extern unsigned int GET_CONTROL ( void );
extern void SET_CONTROL ( unsigned int );
extern void CLR_CONTROL ( unsigned int );
extern void start_l1cache ( void );
extern void stop_l1cache ( void );
extern void invalidate_l1cache ( void );
extern void PrefetchFlush ( void );


unsigned int gmin,gmax;

void do_it ( unsigned int base )
{
    unsigned int ra;
    unsigned int beg,end;
    unsigned int rb;
    unsigned int min,max;
    unsigned int rc;
    
    stop_l1cache(); //just in case
    invalidate_l1cache();

    
    hexstrings(base);
    hexstrings(base);
    hexstrings(base);
    hexstring(base);

    
    hexstring(GET_CONTROL());
    CLR_CONTROL(1<<11);
    hexstring(GET_CONTROL());

    max=0;
    min=0; min--;
    for(ra=base+0x6000;ra<base+0x6100;ra+=4)
    {
        unsigned int flag;
        
        PUT32(ra+0x00,0xe2500001);
        PUT32(ra+0x04,0x1afffffd);
        PUT32(ra+0x08,0xe12fff1e);
        GET32(ra+0x08);
        PrefetchFlush();
        
        for(rc=0;rc<4;rc++)
        {
            beg=GET32(ARM_TIMER_CNT);
            HOP(0x20000,ra);
            end=GET32(ARM_TIMER_CNT);
            rb=end-beg;
            flag=0;
            if(rb>gmax) gmax=rb;
            if(rb<gmin) gmin=rb;
            if(rb>max) { flag++; max=rb; }
            if(rb<min) { flag++; min=rb; }
            if(flag)
            {
                hexstrings(ra);
                hexstrings(rb);
                hexstrings(min);
                hexstrings(max);
                hexstring(max-min);
            }
        }
    }

    hexstring(GET_CONTROL());
    SET_CONTROL(1<<11);
    hexstring(GET_CONTROL());
if(1)
{
    max=0;
    min=0; min--;
    for(ra=base+0x6000;ra<base+0x6100;ra+=4)
    {
        unsigned int flag;
        
        PUT32(ra+0x00,0xe2500001);
        PUT32(ra+0x04,0x1afffffd);
        PUT32(ra+0x08,0xe12fff1e);
        GET32(ra+0x08);
        PrefetchFlush();


        for(rc=0;rc<4;rc++)
        {
            beg=GET32(ARM_TIMER_CNT);
            HOP(0x20000,ra);
            end=GET32(ARM_TIMER_CNT);
            rb=end-beg;
            flag=0;
            if(rb>gmax) gmax=rb;
            if(rb<gmin) gmin=rb;
            if(rb>max) { flag++; max=rb; }
            if(rb<min) { flag++; min=rb; }
            if(flag)
            {
                hexstrings(ra);
                hexstrings(rb);
                hexstrings(min);
                hexstrings(max);
                hexstring(max-min);
            }
        }
    }
}
    hexstring(GET_CONTROL());
    CLR_CONTROL(1<<11);
    hexstring(GET_CONTROL());
            ra=GET32(ARM_TIMER_CNT);


    start_l1cache();

    max=0;
    min=0; min--;
    for(ra=base+0x6000;ra<base+0x6100;ra+=4)
    {
        unsigned int flag;
        
        PUT32(ra+0x00,0xe2500001);
        PUT32(ra+0x04,0x1afffffd);
        PUT32(ra+0x08,0xe12fff1e);
        GET32(ra+0x08);
        PrefetchFlush();

        invalidate_l1cache();
        for(rc=0;rc<4;rc++)
        {
            beg=GET32(ARM_TIMER_CNT);
            HOP(0x20000,ra);
            end=GET32(ARM_TIMER_CNT);
            rb=end-beg;
            flag=0;
            if(rb>gmax) gmax=rb;
            if(rb<gmin) gmin=rb;
            if(rb>max) { flag++; max=rb; }
            if(rb<min) { flag++; min=rb; }
            if(flag)
            {
                hexstrings(ra);
                hexstrings(rb);
                hexstrings(min);
                hexstrings(max);
                hexstring(max-min);
            }
        }
    }

    hexstring(GET_CONTROL());
    SET_CONTROL(1<<11);
    hexstring(GET_CONTROL());

    max=0;
    min=0; min--;
    for(ra=base+0x6000;ra<base+0x6100;ra+=4)
    {
        unsigned int flag;
        
        PUT32(ra+0x00,0xe2500001);
        PUT32(ra+0x04,0x1afffffd);
        PUT32(ra+0x08,0xe12fff1e);
        GET32(ra+0x08);
        PrefetchFlush();

        invalidate_l1cache();
        for(rc=0;rc<4;rc++)
        {
            beg=GET32(ARM_TIMER_CNT);
            HOP(0x20000,ra);
            end=GET32(ARM_TIMER_CNT);
            rb=end-beg;
            flag=0;
            if(rb>gmax) gmax=rb;
            if(rb<gmin) gmin=rb;
            if(rb>max) { flag++; max=rb; }
            if(rb<min) { flag++; min=rb; }
            if(flag)
            {
                hexstrings(ra);
                hexstrings(rb);
                hexstrings(min);
                hexstrings(max);
                hexstring(max-min);
            }
        }
    }


    hexstring(GET_CONTROL());
    CLR_CONTROL(1<<11);
    hexstring(GET_CONTROL());

    stop_l1cache();
}
//-------------------------------------------------------------------------
int notmain ( void )
{
    //unsigned int ra,rb;
    unsigned int ra;
    unsigned int beg,end;

    //uart_init();
    hexstrings(0x12345678);
    hexstrings(0x12345678);
    hexstrings(0x12345678);
    hexstrings(0x12345678);
    hexstring(0x12345678);

    gmax=0;
    gmin=0; gmin--;

    PUT32(ARM_TIMER_CTL,0x00000000);
    PUT32(ARM_TIMER_CTL,0x00000200);

    beg=GET32(ARM_TIMER_CNT);
    ASMDELAY(100000);
    end=GET32(ARM_TIMER_CNT);
    hexstring(end-beg);

    for(ra=0;ra<4;ra++)
    {
        beg=GET32(ARM_TIMER_CNT);
        ASMDELAY(100000);
        end=GET32(ARM_TIMER_CNT);
        hexstring(end-beg);
    }
    start_l1cache();
    for(ra=0;ra<4;ra++)
    {
        beg=GET32(ARM_TIMER_CNT);
        ASMDELAY(100000);
        end=GET32(ARM_TIMER_CNT);
        hexstring(end-beg);
    }
    invalidate_l1cache();
    for(ra=0;ra<4;ra++)
    {
        beg=GET32(ARM_TIMER_CNT);
        ASMDELAY(10);
        end=GET32(ARM_TIMER_CNT);
        hexstring(end-beg);
    }
    invalidate_l1cache();
    for(ra=0;ra<4;ra++)
    {
        beg=GET32(ARM_TIMER_CNT);
        ASMDELAY(10);
        end=GET32(ARM_TIMER_CNT);
        hexstring(end-beg);
    }
    stop_l1cache();

    do_it(0xC0000000);
    do_it(0x80000000);
    do_it(0x40000000);

    hexstrings(gmin); hexstrings(gmax); hexstring(gmax-gmin);
    hexstring(0x12345678);

    return(0);
}

