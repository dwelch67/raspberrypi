
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// this is derived from:

/*
 *  Broadcom BCM2708 watchdog driver.
 *
 *  (c) Copyright 2010 Broadcom Europe Ltd
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 *      BCM2708 watchdog driver. Loosely based on wdt driver.
 */

//------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

//------------------------------------------------------------------------

#define BCM2708_PERI_BASE           0x20000000
#define PM_BASE                     (BCM2708_PERI_BASE + 0x100000) /* Power Management, Reset controller and Watchdog registers */
#define PM_RSTC                     (PM_BASE+0x1c)
#define PM_WDOG                     (PM_BASE+0x24)
#define PM_WDOG_RESET               0000000000
#define PM_PASSWORD                 0x5a000000
#define PM_WDOG_TIME_SET            0x000fffff
#define PM_RSTC_WRCFG_CLR           0xffffffcf
#define PM_RSTC_WRCFG_SET           0x00000030
#define PM_RSTC_WRCFG_FULL_RESET    0x00000020
#define PM_RSTC_RESET               0x00000102

//------------------------------------------------------------------------
void wdog_start ( unsigned int timeout )
{
    unsigned int pm_rstc,pm_wdog;
    /* Setup watchdog for reset */
    pm_rstc = GET32(PM_RSTC);
    pm_wdog = PM_PASSWORD | (timeout & PM_WDOG_TIME_SET); // watchdog timer = timer clock / 16; need password (31:16) + value (11:0)
    pm_rstc = PM_PASSWORD | (pm_rstc & PM_RSTC_WRCFG_CLR) | PM_RSTC_WRCFG_FULL_RESET;
    PUT32(PM_WDOG,pm_wdog);
    PUT32(PM_RSTC,pm_rstc);
}
//------------------------------------------------------------------------
void wdog_stop ( void )
{
    PUT32(PM_RSTC,PM_PASSWORD | PM_RSTC_RESET);
}
//------------------------------------------------------------------------
unsigned int wdog_get_remaining ( void )
{
    return(GET32(PM_WDOG)& PM_WDOG_TIME_SET);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
