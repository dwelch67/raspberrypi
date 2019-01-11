#include <stdint.h>

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

extern void mmu_init ( uint32_t );
extern void mmu_domain ( uint32_t );
extern void stop_mmu ( void );
extern void invalidate_tlb ( void );

extern void uart_init ( void );
extern void uart_send ( unsigned int );

extern void hexstrings ( unsigned int );
extern void hexstring ( unsigned int );

#define MMUTABLEBASE 0x00004000

#define CACHEABLE 0x08
#define BUFFERABLE 0x04

/**
 * \brief creates an translation table entry (for sections of size 1 MiB)
 * \param[in] virtual the virtual address (only top 12 bits used)
 * \param[in] physical the physical address (only top 12 bits used)
 * \param[in] flags the flags for the section
 **/
uint32_t mmu_section ( uint32_t virtual, uint32_t physical, uint32_t flags )
{
    uint32_t offset = virtual >> 20;
    // plus and or are the same thing here, as MMUTABLEBASE is 14 bit aligned
    uint32_t* entry = (uint32_t*) (MMUTABLEBASE | (offset<<2));
    
    // mask lower 20 bits of physical address then ORR flags and 0x02 for 1 MiB
    uint32_t physval = (physical & 0xfff00000) | (flags & 0x7ffa) | 0x02; 

    *entry = physval;
    return(0);
}


/**
 * \brief creates an translation table entry (for sections of size 1 MiB)
 * \param[in] virtual the virtual address (only top 12 bits used)
 * \param[in] physical the physical address (only top 12 bits used)
 * \param[in] flags the flags for the section
 **/
uint32_t mmu_page ( uint32_t virtual, uint32_t physical, uint32_t flags, uint32_t secondbase )
{
    uint32_t offset = virtual >> 20;
    // plus and or are the same thing here, as MMUTABLEBASE is 14 bit aligned
    uint32_t* entry = (uint32_t*) (MMUTABLEBASE | (offset<<2));
    // mask lower 20 bits of physical address then ORR flags and 0x01 for coarse translation
    uint32_t entryval = (secondbase & 0xfffffc00) | (flags & 0xf0) | 0x01; 

    // set first level descriptor
    *entry = entryval;
    
    // mask everything except bits 19:12
    offset = (virtual >> 12) & 0xff;
    // form the second level
    uint32_t* secondLevelEntry = (uint32_t*) ((secondbase & 0xfffffc00) | (offset << 2));
    
    // form the value of the second level descriptor
    // bytes 31:12 are the page base address, flags contain B,C, AP_x = 0b11 
    // for all and the 0x02 at the end to identify the entry as small page
    uint32_t physval = (physical & 0xfffff000) | 0xff0 | (flags & 0xc) | 0x02;
    
    // set the second level descriptor
    *secondLevelEntry = physval;
    return(0);
}

    
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    uart_init();
    hexstring(0xDEADBEEF);

    PUT32(0x00045678,0x00045678);
    PUT32(0x00145678,0x00145678);
    PUT32(0x00245678,0x00245678);
    PUT32(0x00345678,0x00345678);

    PUT32(0x00346678,0x00346678);
    PUT32(0x00146678,0x00146678);

    PUT32(0x0AA45678,0x12345678);
    PUT32(0x0BB45678,0x12345678);
    PUT32(0x0CC45678,0x12345678);
    PUT32(0x0DD45678,0x12345678);

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

    for(ra=0;;ra+=0x00100000)
    {
        mmu_section(ra,ra,0x0000);
        if(ra==0xFFF00000) break;
    }

    //mmu_section(0x00000000,0x00000000,0x0000|8|4);
    //mmu_section(0x00100000,0x00100000,0x0000);
    //mmu_section(0x00200000,0x00200000,0x0000);
    //mmu_section(0x00300000,0x00300000,0x0000);
    //peripherals
    mmu_section(0x20000000,0x20000000,0x0000); //NOT CACHED!
    mmu_section(0x20200000,0x20200000,0x0000); //NOT CACHED!

    mmu_init( MMUTABLEBASE );

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

    mmu_section(0x00100000,0x00300000,0x0000);
    mmu_section(0x00200000,0x00000000,0x0000);
    mmu_section(0x00300000,0x00100000,0x0000);
    invalidate_tlb();

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

    mmu_page(0x0AA45000,0x00145000,0,0x00000400);
    mmu_page(0x0BB45000,0x00245000,0,0x00000800);
    mmu_page(0x0CC45000,0x00345000,0,0x00000C00);
    mmu_page(0x0DD45000,0x00345000,0,0x00001000);
    mmu_page(0x0DD46000,0x00146000,0,0x00001000);
    //put these back
    mmu_section(0x00100000,0x00100000,0x0000);
    mmu_section(0x00200000,0x00200000,0x0000);
    mmu_section(0x00300000,0x00300000,0x0000);
    invalidate_tlb();

    hexstring(GET32(0x0AA45678));
    hexstring(GET32(0x0BB45678));
    hexstring(GET32(0x0CC45678));
    uart_send(0x0D); uart_send(0x0A);

    hexstring(GET32(0x00345678));
    hexstring(GET32(0x00346678));
    hexstring(GET32(0x0DD45678));
    hexstring(GET32(0x0DD46678));
    uart_send(0x0D); uart_send(0x0A);

    //access violation.
    mmu_domain ( 0xffffff03 );
    mmu_section(0x00100000,0x00100000,0x0020);
    invalidate_tlb();

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

    hexstring(0xDEADBEEF);

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2014 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
