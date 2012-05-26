
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define SYSTIMERCLO 0x20003004
#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028

//0x01000000 17 seconds
//0x00400000 4 seconds
#define TIMER_BIT 0x00400000

//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    ra=GET32(GPFSEL1);
    ra&=~(7<<18);
    ra|=1<<18;
    PUT32(GPFSEL1,ra);

    while(1)
    {
        PUT32(GPSET0,1<<16);
        while(1)
        {
            ra=GET32(SYSTIMERCLO);
            if((ra&=TIMER_BIT)==TIMER_BIT) break;
        }
        PUT32(GPCLR0,1<<16);
        while(1)
        {
            ra=GET32(SYSTIMERCLO);
            if((ra&=TIMER_BIT)==0) break;
        }
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
