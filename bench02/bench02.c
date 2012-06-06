
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

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
extern void ARMTEST0 ( unsigned int );
extern void ARMTEST1 ( unsigned int );
extern void ARMTEST2 ( unsigned int );
extern void ARMTEST3 ( unsigned int );
extern void THUMBTEST0 ( unsigned int );
extern void THUMBTEST1 ( unsigned int );
extern void THUMBTEST2 ( unsigned int );

//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int beg,end;

    uart_init();
    hexstring(0x12345678);

    start_l1cache();

    init_timer();
    beg=timer_tick();
    ARMTEST0(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    ARMTEST0(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    ARMTEST1(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    ARMTEST2(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    ARMTEST3(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    THUMBTEST0(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    THUMBTEST1(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    init_timer();
    beg=timer_tick();
    THUMBTEST2(0x01000000);
    end=timer_tick();
    hexstring(end-beg);

    stop_l1cache();

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
