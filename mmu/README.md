# MMU on Raspberry Pi

See the top level README file for more information on documentation
and how to run these programs.

## Preface

This example demonstrates ARM MMU basics.

You will need the [ARM ARM] (ARM Architectural Reference Manual) for
ARMv5.

This code so far does not work on the Raspberry pi 2 yet, will get
that working at some point, the knowledge here still applies, I expect
the differences to be subtle between ARMv6 and 7 but will see.

## Fundamentals

A Memory Managment Unit (MMU) translates virtual addresses into physical 
addresses, as well as checking access permissions and giving control over 
marking regions cacheable. This allows the programmer to identify the memory 
regions which may be cached for faster access in the CPU core, but leaving out 
for example hardware registers, which are mapped into memory. 

There is a boundary inside the chip around the ARM-core. The ARM-core itself 
uses virtual addresses for memory and hardware-accesses, which are translated 
by the MMU to physical addresses when actually requesting a value in memory. 
Every access to the memory or the "world side" has to go through the MMU. 

When the ARM-core powers up the MMU is disable, which means that every access 
will pass through unmodified, making the virtual addresses (processor side) 
equal to the physical addresses (world side). All of the example thus far in 
this repository (e.g. blinkers) are based in physical addresses. 

We already know that somewhere else in the chip the used addresses are 
different. The Raspberry Pi manual is written for 0x7Exxxxxx based addresses, 
but for the ARM's physical addresses for the same things is 0x20xxxxxx for the 
Raspberry Pi 2 and 0x3Fxxxxxx for the Raspberry Pi 2. For this discussion we 
only care about the ARM and the ARM MMU, not for the other mystical translation 
on the chip.

### Motivation

Let's say I am programming a program for let's say Linux. I would have to link 
my program to use specific addresses (or a specific address space). Let's 
assume, that our program is loaded into 0x8000 and it can use the memory from 
0x0000 onwards. That would be fine for one program, but let's say another 
program wants to be loaded to 0x8000 or maybe use this space as memory for 
variables. So how can we run several program without the risk of them 
clobbering each other?

The answer is neither is actually loaded into 0x8000 when running. The programs 
may assume, that they can use the addresses like stated above, but in reality 
the addresses when requesting memory cells will be translated by the MMU. So 
one program could be placed to 0x10008000, the other one at 0x20008000. When 
program 1 thinks it accesses 0x0000abcd it is really accessing 0x1000abcd, the 
other one 0x2000abcd. This translation is completely transparent to the 
programs, i.e. they will never notice, that the addresses are translated for 
them.

Theoretically you could assign every virtual address a physical address to be 
translated to, but that does not make much sense. The ARM-core used on the 
Raspberry Pi is a 32-bit processor, i.e. it uses 32-bit addresses. This means 
we have 4 Giga (2^32) addresses. A table containing the physical addresses 
alone would be 16 GB big. 

The ARM has one option to translate the top 12 bits of the virtual address to 
the top 12 bits of the physical address, leaving the lower 20 bits as they are 
between the virtual and physical space. This means we can control 1 MB of 
address space per definition and have 4096 entries in a table somewhere to 
convert virtual to physical addresses. The ARM still uses all 32 Bits, 12 for 
the top 12 address bits, the other ones as control flags. One of them indicates 
whether a region is marked as cacheable. 

### About caching

A cache is a (very small but) very fast memory inside the processor. It is used 
by the processor transparently to remember data which is loaded and/or stored 
by your program together with its address. This behaviour saves the processor 
from having to request the value from RAM every time it is needed, having to 
wait for the (slower) memory on every read/write. Caching can vastly increase 
the speed of your program. Changes to values are written through the cache.

But why is it disabled with the MMU disabled? Let's assume we want to read the 
value register of a timer. This is done by reading from a specific address. 
What we want would be, that we get the current value every time we read the 
register. When caching is enabled for these memory regions we would read the 
current value one time, but after that we will only get the cached value. This 
is no good, because you cannot control peripherals if you are unable to get the 
current state or value of a peripheral, because the cache only gives you the 
last (old) values. 

Likewise, if you have some RAM, which is shared by more than one resource, like 
the GPU and the ARM or several processor cores on the Raspberry Pi 2 or 3, you 
will have a similar situation. In general you want to disable caching on every 
region which can be modified by other means than through the cache. The MMU 
let's you enable or disable caching on memory regions.

### About access permissions

Let's think back to our example with the two programs running "at the same 
time". You don't want any of the programs to get access to the operating 
systems data structures nor do you want any of the two modifying code or data 
of the other program. You would not want a video game to get access to your 
banking account open in another window, would you? 

The mechanisms vary by processor family but the MMU provides the security 
mechanisms. When a particular program is running on the processor there are 
mean to allow or restrict access to specific memory spaces. If some code 
accesses an address it does not have the permissions for, then a Data 
Abort-Exception happens, and the processor will stop running the code of that 
application. The Operating System will be notified (by the means of an 
Exception Handler / Interrupt Service Routine). 

This Data Abort does not have to be fatal for the application, but it could be 
by design. Think of a virtual machine, running on the processor and when it 
tries to access its peripherals, the real Operating System can be notified to 
simulate the peripheral and keep the virtual machine running. 

### About virtual memory

What happens when you run out of memory on your computer? Let's say the RAM is 
use up completely, but an application uses for example `malloc` to request more 
memory. The operating system will then find a block of memory of another 
application and save that to disk. This space can then be used by the running 
application as memory. When the other program then tries to use the swapped out 
memory, it will trigger an Data Abort-Exception in the processor. This will 
trigger the operating system to swap that memory block back into memory (maybe 
substituting another block of another application).

The term swap comes from the idea that these blocks of ram are swapped back and 
forth to disk, program A's ram goes to disk and is swapped with program T's, 
then program T's is swapped with program K's and so on. This is why starting 
right after you venture off that edge from real RAM to virtual, your computers 
performance drops dramatically and disk activity goes way up. The more things 
run the more memory needs to be swapped onto the much slower disk.

## Wading through the documentation 

I am mostly using the ARMv5 Architectural Reference Manual DDI0100I. ([ARM ARM])

Unfortunately the ARM ARM does not look the same from one to the next. With the 
Raspberry Pi 1 and Zero we are technically using an ARMv6 (architecture version 
6), but when we go to ARM's website, there is an ARMv5, ARMv7 and ARMv8-version, 
but no ARMv6. The ARMv5 manual is actually the original ARM ARM, where they (I 
assume) realized, that they could not maintain all the architecture variations 
in one document forever. So they split them per revision. With respect to the 
MMU the ARMv5 manual cover the ARMv4, ARMv5 and ARMv6. There is a mode where 
you can have the same code and table to work on all three, so you don't have to 
if-then-else your code based on whatever architecture you find. This example is 
based on this legacy mode with subpages enables.

The 1 MB sections mentioned above are called sections. The ARM MMU also has 
blobs with a smaller size of 4096 bytes, which are called small page. I will 
touch on those two sizes.

As mentioned above the Raspberry Pi has a 32 bit address space. 1 MB sections 
means 20 bits unaltered (bits 32 to 20) and 12 bits translated meaning 4096 
1 MB sections, i.e. 4096 entries in the table. The top 12 bits of the virtual 
address get translated to the top 12 bits of the physical address. There are no 
additional rules on the translation, you can have for example
- virtual = physical
- any combination you like
- have a bunch of virtual sections point to the same physical space.

ARM uses the term Virtual Memory System Architecture or VMSA and
they say things like VMSAv6 to talk about the ARMv6 VMSA. There
is a section in the ARM ARM titled Virtual Memory System Architecture.
In there we see the coprocessor registers, specifically CP15 register
2 is the translation table base register. In the CP15 subsection register 2 is 
the translation table base register. There are three opcodes which give us 
access to three things: `TTBR0`, `TTBR1` and the control register.

### Writing the Translation table base address

First we read this comment (pg. 741, heading: Register 2: Translation table 
base):
> If N = 0 always use `TTBR0`. When N = 0 (the reset case), the translation
> table base is backwards compatible with earlier versions of the
> architecture.

So we want to leave N = 0 and use `TTBR0`.

The `TTBR0`-register contains the base address in the physical address space. 
The bits 31 down to 14-n (with n=0 in our case) are used as the base address. 
Note that the MMU cannot go through the MMU to figure out how to go through the 
MMU. It operates exclusively in physical address space and has direct access to 
memory. In a second we are going to see, that the base address for the MMU table 
has to be aligned to 16384 bytes (2^14), the lower 14 bits of our TLB base 
address is all zeroes (TLB=Translation Look-Aside Buffer)). 

We write that register using

```armasm
    mcr p15,0,r0,c2,c0,0 ;@ tlb base
```

#### The co-processor

Let me explain what that mnemonic does. `mcr` is a special instruction to write 
to registers of the co-processor. This co-processor manages loads of functions 
of the ARM-core, like unaligned data access or the MMU. You probably already 
came across `msr` which is an instruction to store data into the status 
register of the ARM-core (for example to set a new privilege mode or to enable 
interrupts). You cannot access the status register or the co-processor 
registers with the normal `mov` instruction.

The co-processor has several registers, which can be accessed by the `mcr` or 
the `mrc` instruction. The parameters are:

```armasm
    MCR{cond} P15,<Opcode_1>,<Rd>,<CRn>,<CRm>,<Opcode_2>
    MRC{cond} P15,<Opcode_1>,<Rd>,<CRn>,<CRm>,<Opcode_2>
```

[co-processor] shows the list of registers plus their assignment to the 
parameters for the two instructions. So the above statement will access the following parameters:
- Opcode_1: 0
- Rd: r0
- CRn: c2 (register number within CP15)
- CRm: c0 (operational register)
- Opcode_2: 0

r0 serves as source register of the value to be written to the Register 
identified by (c2,c0,0,0).

#### About TLB

As far as we are concerned think of the TLB as an array of 32 bit integer, 
each one being used to translate a virtual to a physical address and 
describes permissions and caching. My example is going to have a define called 
`MMUTABLEBASE` which will be where out TLB table starts. The TLB is used as 
cache for the page tables.

The MMU is completely realised in hardware, but you can configure it the way 
you want. It will operate on the values we set into our page table with Or and 
And-operations (i.e. bit-manipulations). It uses portions of the virtual 
addresses to find the correct plane in the page table to find the according 
physical address. From the next bits it will decide what to do next. We, as 
programmers, need to know how the MMU calculates the place, so we can put our 
descriptor into the correct space, so the MMU finds it.

#### Translating virtual to physical addresses

In the manual there is figure B4-4 (page B4-29), which shows a diagram of how 
the addresses are translated. It uses X instead of N (which we want to be 0). 
The modified virtual address in this diagram is, as far as we are concerned, 
unmodified as we want to intend to use our virtual addresses. The first thing 
we see, is that the lower 14 Bits of the translation table base (in my example 
`MMUTABLEBASE`), i.e. the start address of the translation table are marked as 
SBZ, i.e. should-be-zero. This means, that the translation table should be 
aligned to 16 KiB (2^14 Bytes). Using 0x0000 as starting address would not make 
much sense, as this is the place for the interrupt vector. The next good place 
would be 0x00004000. Adding another 16 KiB to that address is (not a 
coincidence) 0x8000, where we put our code. But any other address, which is 
aligned to 16 KiB should work, as long as you have enough memory there and not 
clobber anything else. 

The figure B4-4 shows, that we take the top 12 bits of the virtual address, 
multiply by 4 (or shift left by 2) and add that to the translation table base, 
which gives the address of the first level descriptor for that virtual address. 
A multiplication by 4 is no coincidence, but rather takes the length of each 
descriptor into account (which is exactly 32 bit, or 4 byte). The descriptor 
is fetched and interpreted. As long as we leave N=0 the MMU will always look 
into the first 12 bits, which replace the first 12 bits of the virtual address. 
The last two bits of the descriptor are flags, if they are 0b10, then it is a 
1 MB section. If it's something different, then a second level translation 
will be triggered, but for now let's focus on the simpler part.

##### An example

```
    MMUTABLEBASE                    = 0x00004000
    virtual address                 = 0x12345678
    -> first 12 bit (moved to right): 0x00000123
    -> multiplied by 4:               0x0000048c
    descriptor for the section:       0x0000448c
    let's assume the descriptor was   0xABC00002
    -> physical address               0xABC45678
```

#### 16 MiB Supersections

So the ARM ARM states, that you can have 16 MiB supersections. This would make 
life easier, right? Well, no. You still have to generate 16 descriptors for 
each of the possible 1 MiB sections, so you might as well make them 1 MiB big. 
You can read up on the differences and try the supersections our, but I'm going 
to use 1 MiB sections for now.

Maybe you figured out a bit of a problem here. Every load and store with the MMU
enabled requires at least one MMU table lookup. The MMU memory accesses of 
course don't have to go through the MMU, but every other store or load. This
does have a performance hit. Therefore the MMU caches the last TLB-lookups. 
This helps but the conversion has to be done on every requested address. 

### Descriptor format

I am looking on the subsection about First Level Descriptors in the ARM ARM,
especially the Table B4-1 (pg. B4-27, First-level descriptor format (VMSAv6, 
subpages enabled)). 

This table identifies four different sets of last two bits [1:0]:
- `0b00` - this section is unmapped. Attempting to access these addresses will generate a translation fault (Data Abort). The bits [31:2] are ignored by the hardware, although it is recommended to keep valid permissions for the descriptor there.
- `0b01` - for coarse second level table, second level lookup required for translation; allows more fine grained sectioning of the section
- `0b10` - sections descriptor for its associated virtual addresses, no second level lookup
- `0b11` - reserved in VMSAv6

For now let's work with the `0b10`-entries. The format of the entry is as follows:

|   | Bits 31:20 | Bits 19:15 | 14,13,12 | 11,10 | 9 | 8:5 | 4 | 3 | 2 | 1,0 |
|---|------------|------------|----------|-------|---|-----|---|---|---|-----| 
| Section | Section base address | SBZ | TEX | AP | IMP | Domain | SBZ | C | B | `10` |

The **section base address** are the 12 top bits of the physical address, which 
substitute the top 12 bits of the virtual address. The **C** bit marks the address 
region as cacheable. We do absolute not want to cache peripheral regions, 
RAM-regions maybe. The C-flag simply asserts bits on the AMDA/AXI (memory) bus 
that marks the transaction as cacheable, you still need a cache setup and 
enabled for the transaction to actually get cached. If you don't have the 
cache for that transaction type enabled, then it just does a normal memory (or
peripheral) operation.

The **B** bit, means bufferable, as in write buffer. This enables a "cache" 
but for writing instead of reading. When writing a value to RAM (or peripheral) 
everything is known, the data and the address. The buffer-bit allows some logic 
at this level to accept the value and address and continue to write the data to 
the slower RAM or peripheral (or cache) and let's the CPU go on executing it's 
program. This may give us a performance boost. When a second write appears and 
we only have a single place for a transaction, the processor gets stalled until 
the first one is complete and the second write-command can be saved to the 
buffer. The advantage is that for a number of writes the processor can hand the 
needed data to the memory controller and carry on.

You need to look up the **TEX** bits yourself. I will stick to them being 0b000 
and will not mess with any fancy features here. The combinations of TEX, C and 
B bits make some subtle differences, look them up in Table B4-3 (CB + TEX 
Encodings). 

The **AP** bits indicate the level of access permissions (see Table B4-1 MMU 
access permissions, pg. B4-9), for page table formats, which don't support APX, 
value 0 is assumed. The following AP-values are therefore valid:
- `0b00` - No access for anyone; will generate permission fault on every access
- `0b01` - Read/Write permission for privileged mode 
- `0b10` - Read/write for privileged mode, read for user mode (writes in user mode trigger permission faults)
- `0b11` - Full access (R/W for everyone)

The **domain** is a bit trickier to explain. There is a register right next to the translation table base address register which contains 16 different domain specifications. These definitions are 2 bit long each:
- `0b00` - **no access**, any access generates a domain fault
- `0b01` - **client**, accesses are checked against the access permission bits in the TLB entry
- `0b10` - **UNPREDICTABLE** behaviour
- `0b11` - **manager**, accesses are not checked and cannot generate a permission fault

The domains basically are 16 different definitions which control the behaviour 
on access. We can define for example 16 types of applications and assign them 
sections. We assign sections to domains by setting the four **domain** bits of 
the translation table entry to the number of the definition in the register. 
With changing to bits in this register we can then put sections of a domain 
into another permission mode, which is quite useful, because we don't need to 
change the MMU table.

For starters we are going to set all of the domains to `0b11` don't check and 
all of our sections can have the domain number 0. 

## A simple implementation

```c
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
        uint32_t* entry = MMUTABLEBASE | (offset<<2);
        
        // mask lower 20 bits of physical address then ORR flags and 0x02 for 1 MiB
        uint32_t physval = (physical & 0xfff00000) | (flags & 0x7ffa) | 0x02; 

        *entry = physval;
        return(0);
    }

    #define CACHEABLE 0x08
    #define BUFFERABLE 0x04
```

### Filling the table with sections

Before enabling the MMU itself we need to make sure, that every section of 
memory we want to use is defined with a valid entry in the table. If not, 
access to that region will trigger a fault handler - if you decide to write 
one. Which in turn can access to non mapped memory - which is not good. 

The smallest amount of RAM on a Raspberry Pi is 256 MiB or 0x10000000 bytes. 
Our program enters at address 0x8000, so that is within the first
section 0x000xxxxx so we should make that section cacheable and
bufferable.

```c
    mmu_section( 0x00000000,0x00000000, CACHEABLE | BUFFERABLE );
```

This statement will create an entry for the virtual address space 0x000xxxxx 
to the physical addresses 0x000xxxxx enable the cache and write buffer. If we 
want to use all 256mb we would need to do this for all the sections from 
0x000xxxxx to 0x100xxxxx.



We know that for the Raspberry Pi 1 the peripherals, like AUX / UART and such 
are in ARM physical space at 0x20xxxxxx. To allow for more RAM on the Raspberry 
Pi 2 they needed to that peripheral base address and moved it to 0x3Fxxxxxx. 
We can either create 16 1 MiB section entries to cover the whole range of 
peripherals or we only define the sections we care to talk to. The UART and the 
GPIO are associated with the 0x202xxxxx space. There are a couple of timers 
in the 0x200xxxxx space so one entry can cover those.

``` c
    mmu_section(0x20000000,0x20000000,0x0000); //NOT CACHED!
    mmu_section(0x20200000,0x20200000,0x0000); //NOT CACHED!
    mmu_section(0x3F000000,0x3F000000,0x0000); //NOT CACHED!
    mmu_section(0x3F200000,0x3F200000,0x0000); //NOT CACHED!
```

These sections are not cached and not buffered, but but we may play with that 
to demonstrate what caching a peripheral can do to you, why we need to turn on 
the MMU if for no other reason than to get some bare metal performance by 
using the cache.

Now you have to think on a system level here, there are a number
of things in play. We need to plan our memory space, where are we
putting the MMU table, where are our peripherals, where is our program.

If the only reason for using the MMU is to allow the use of the cache
then just map the whole world 1:1; if you want with the peripherals not cached 
and the rest cached.

### Cache invalidation

So once our tables are setup then we need to actually turn the
MMU on. When initialising a cache to you want to clean out all the entries in a 
safe way. You want to invalidate everyhting, mark every cache line as empty / 
available. Likewise you want the TLB caching the MU does to be invalidated, 
so the MMU starts up with no valid lines in the cache, that don't match our 
entries. Also we want the CPU to do a Data Synchronization Barrier (DSB), so 
every explicit memory transaction is finished before the next instruction begins. 

All of the above can be done using the C15 [co-processor]. So, to summarise:
1. Invalidate all caches (Instruction and data, write 0 to `0, c7, c7, 0`)
2. Invalidate the TLB entries (write 0 to `0, c8, c7, 0`)
3. Data synchronisation barrier (write 0 to `0, c7, c10, 4`)
4. Set the domain access controls (write 0xffffffff to `0, c3, c0, 0`, 0b11 for every domain)
5. Set the base address for the translation table (`0,c2,c0,0`)
6. Enable level 1 caches and the MMU in the control register (`0,c1,c0,0`) and some other useful things:
  - bit 0 (M) enables MMU
  - bit 2 (C) enables level 1 data cache
  - bit 11 (Z) enables branch prediction
  - bit 12 (I) enables instruction cache
  - bit 22 (U) enables non-aligned data access as well as mixed big-/little-endian data access

What bits of these you want to set is up to you. I would recommend M, C and I, 
but I am going ahead and set them all. So simple example code implementing the 
the MMU-enabling process might be this: 

```armasm
    .global mmu_init
    mmu_init:
        mov r1,#0
        // invalidate caches
        mcr p15,0,r1,c7,c7,0 
        // invalidate TLB entries
        mcr p15,0,r1,c8,c7,0 
        // data synchronisation barrier
        mcr p15,0,r1,c7,c10,4 
        
        // set all domains to 0b11
        ldr r1, =0xffffffff
        mcr p15,0,r1,c3,c0,0
        
        // set the translation table base address (remember to align 16 KiB!)
        mcr p15,0,r0,c2,c0,0
        
        // set the bits mentioned above
        ldr r1, =0x00401805
        mrc p15,0,r2,c1,c0,0
        orr r2,r2,r1
        mcr p15,0,r2,c1,c0,0
        
        mov pc, lr
```

For messing with the translation tables after the MMU is started, you will need
to invalidate the TLB cache again, so let's put this part into its own function.
We don't need to care about the L1 cache, this time. Also ARMv6 introduces a 
feature to help with invlidating the TLB, but I'm going with this solution:

```armasm
    .globl tlb_invalidate
    tlb_invalidate:
        mov r2,#0
        // invalidate TLB entries
        mcr p15,0,r1,c8,c7,0 
        // data synchronisation barrier
        mcr p15,0,r1,c7,c10,4 
        mov pc,lr
```

#### (JTAG) Debugging and caching

Something to note here. Debugging using the JTAG based on-chip-debugger
makes life easier. No SD-card swapping and no more EEPROM-flashing. BUT,
it is not completely without issue. The basic problem is that caches often 
seperate instruction fetches from data reads and writes. Let's say you execute 
instructions at 0xD0000 (which is cached) and an instruction 0xC000. So you
transfer your programm, set the start address and run again. 

For each instruction in the program the prior instruction in that address might 
still be in the instruction cache and the new one in main RAM (or data cache). 
So, when running the new program you might still be running the old 
instructions, which are fetched back from the instruction cache, not the RAM, 
unless an invalidate or flush happens).

There is also the question of are the instruction and data caches shared? 
They can be specific to the core and your setup. Is your JTAG-debugger able to 
disable the caches, has it done that for you, or can you do it manually.

Any time you are using the instruction or data caches you need to be careful 
using a JTAG-debugger or even a bootloader type approach depending on its
design as you might end up doing data writes of instructions and going
around the instruction cache or worse. This may be done by your JTAG debugger,
but keep in mind to change back to / from Raspberry Pi 2 when switching between 
the Pis. Otherwise this might driver you mad, when you keep downloading new 
code but the Pi crashes or behaves unexpectedly.

## Having fun with address translation

So the example is going to start with the MMU off and write to
addresses in four different 1MB address spaces, so we can play with the section 
descriptors and demonstrate virtual to physical address conversion later.

```c
    // write data to four different 1 MiB sections
    PUT32(0x00045678,0x00045678);
    PUT32(0x00145678,0x00145678);
    PUT32(0x00245678,0x00245678);
    PUT32(0x00345678,0x00345678);

    // write the data back to UART
    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    // 0D    CR  '\r' (carriage ret)
    uart_send(0x0D); 
    // 0A    LF  '\n' (new line)
    uart_send(0x0A);

    // Then setup the MMU with at least those four sections
    mmu_section(0x00000000,0x00000000,CACHEABLE | BUFFERABLE);
    mmu_section(0x00100000,0x00100000,0x0000);
    mmu_section(0x00200000,0x00200000,0x0000);
    mmu_section(0x00300000,0x00300000,0x0000);
    
    //  and the peripherals:
    mmu_section(0x20000000,0x20000000,0x0000); //NOT CACHED!
    mmu_section(0x20200000,0x20200000,0x0000); //NOT CACHED!

    // Start the MMU with the instruction and data caches enabled:
    mmu_init ( MMUTABLEBASE );

    // when we read those four addresses back we get the same output
    // as we wrote before because we mapped virtual = physical
    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);
```

But what if we swizzle things around? Don't mess with the 0x00000000-section, 
because that is where our code is.

```c
    // change the table entries
    mmu_section(0x00100000,0x00300000,0x0000);
    mmu_section(0x00200000,0x00000000,0x0000);
    mmu_section(0x00300000,0x00100000,0x0000);

    // invalidate the TLB
    invalidate_tlbs();

    // and read the addresses again, which we wrote to above
    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);
```

The 0x000xxxxx entry was not modified, so we get 0x000045678 as the output. The
section 0x001xxxxx will read from physical addresses 0x003xxxxx so we get the 
0x00345678 output, 0x002xxxxx will translate to the 0x000xxxxx space
so that read gives 0x00045678 and the 0x003xxxxx is mapped to physical 
0x001xxxxx giving 0x00145678 as the output.

So up to this point the output looks like this:

```
    00045678
    00145678
    00245678
    00345678

    00045678
    00145678
    00245678
    00345678

    00045678
    00345678
    00045678
    00145678
```

The first block is with the MMU disabled, the second one with MMU enabled but 
1:1 virtual to physical translation, the third one with the non 1:1 translation.

## Coarse paging

With coarse paging the logic does not know what kind of translation will be done
until the first level read, so the first step is identical to the translation
above. If it sees a `0b01` as the lower 2 bits of the first level descriptor, 
then it knows, it's a coarse page entry and it needs as second level fetch. 
Table B4-5 Accessing coarse page table second-level descriptors (pg. B4-30) 
shows the logic fetching the second level descriptor. 

### Second level descriptor format

There are two things to the two level translation. At first we need to set the 
first level descriptors accordingly. The format is as follows Table B4-1 
(pg. B4-27, First-level descriptor format (VMSAv6, subpages enabled)).

|   | Bits 31:10 | 9 | 8:5 | 4,3,2 | 1,0 |
|---|------------|---|-----|-------|-----|
| Coarse Page Table | Coarse Page Table base Address | IMP | Domain | IMP | `01` |

The bits 4:2 are implementation defined and should be zero (SBZ) for VMSAv6. 
The domain-bits are used as above. The bits 31:10 are used as the base address 
of the second level page table. This second level page table needs to be 
aligned to 1 KiB in memory. 

So after fetching the first level descriptor the bits 31:10 of the entry will 
be used as bits 31:10 of the second level descriptor, i.e. it will be used as 
base address for the second level table. The virtual address bits 19:12 will be
used to navigate inside that second level table, i.e. shift completely to the 
right, multiplied by 4 then used added onto the base address. The lowest 2 bits
of the address of the second level entry is always zero. Note that there are 
256 possibilities to fill the bits 19:12 of the virtual address, i.e. the 
section is divided into 256 parts, i.e. 4 KiB pages.

The second level descriptor looks like this (for small pages, for more see 
Table 4-3 Second level descriptor format (subpages enabled), pg. 4-31):

|   | Bits 31:12 | 11,10 | 9,8 | 7,6 | 5,4 | 3 | 2 | 1,0 |
|---|------------|-------|-----|-----|-----|---|---|-----|
| Small page | Small page base address | AP3 | AP2 | AP1 | AP0 | C | B | `10` |

Note, that there are four **AP**-fields here. The small page is divided further 
into four blocks of the same size (i.e. in the case of small pages 1 KiB), which
have their own AP-Access control. AP0 applies to the block with the lowest base 
address. You can set them all to `0b11` - full access and not care, or have 
fine grained access control over the blocks of that page.

### An example

This example is a bit crazy, as the address of the second level descriptor is
an address, where we don't even have RAM anymore on the Raspberry Pi. Normally
you want to keep your second level tables somewhere near the first level table, 
so you have the memory managment information in a confined space. 

```
    MMUTABLEBASE                          = 0x00004000
    virtual address                       = 0x12345678
    -> bits 19:12 of the virtual address:   0x00000045
    address of the first level descriptor:  0x0000448c
    let's assume a descriptor value of:     0xABCDE001
    base address of second level table:     0xABCDE000
    offset to the second level table:       0x00000114
    address of the second level descriptor: 0xABCDE114
```

### A simple implementation

```c
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
        uint32_t* entry = MMUTABLEBASE | (offset<<2);
        // mask lower 20 bits of physical address then ORR flags and 0x01 for coarse translation
        uint32_t entryval = (secondbase & 0xfffffc00) | (flags & 0xf0) | 0x01; 

        // set first level descriptor
        *entry = entryval;
        
        // mask everything except bits 19:12
        offset = (virtual >> 12) & 0xff;
        // form the second level
        uint32_t* secondLevelEntry = (secondbase & 0xfffffc00) | (offset << 2);
        
        // form the value of the second level descriptor
        // bytes 31:12 are the page base address, flags contain B,C, AP_x = 0b11 
        // for all and the 0x02 at the end to identify the entry as small page
        uint32_t physval = (physical & 0xfffff000) | 0xff0 | (flags & 0xc) | 0x02;
        
        // set the second level descriptor
        *secondLevelEntry = physval;
        return(0);
    }
```

So let's assign some sections to coarse translation:

```c
    mmu_small(0x0AA45000,0x00145000,0,0x00000400);
    mmu_small(0x0BB45000,0x00245000,0,0x00000800);
    mmu_small(0x0CC45000,0x00345000,0,0x00000C00);
    mmu_small(0x0DD45000,0x00345000,0,0x00001000);
    mmu_small(0x0DD46000,0x00146000,0,0x00001000);
    
    invalidate_tlb();
```

Let's look in the last two `mmu_small`-statements here. you will notice, that
the `secondbase`-parameter is the same here. This is in fact wanted, as I want 
to add an entry into the secondary table I assigned before, not set a new one, 
i.e. orphaning the old one. So let's assume I would set a new secondary table 
base address like this:

```c
    mmu_small(0x0DD45000,0x00345000,0,0x00001000);
    mmu_small(0x0DD46000,0x00146000,0,0x00001400);
```

When I try to access an address in the page 0x0DD45xxx, then the MMU would look
inside a secondary table located at 0x00001400, which of course does not contain 
our previously set entry for the small page. But it will definitely find 
something there, and probably behave unexpected, if we are not aware of our 
mistake here. So always make sure the secondary table base addresses of the 
pages in the same section are the same. 

## Access violation

First we want to set a domain to 0x00, so accessing a section with that domain 
will definitely trigger an access violation. I will assume we wrote that to 
domain number 1. 

```c
    // set the domain of a section to 0x01
    mmu_section(0x00100000,0x00100000,0x0020);
    invalidate_tlb();

    // then read the data from the sections
    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);
```

I expect that second read-statement to trigger a Data Abort-Exception, so I 
want to write an exception handler, to read the status information of that 
exception. We need to following registers of the C15 co-processor:
- `0,c5,c0,0` - Data Fault Status Register
- `0,c5,c0,1` - Instruction Fault Status Register

### Data Fault Status Register

This register holds the source of the last data fault. The bits have the 
following functions:

| Bit 31:13 | 12 | 11 | 10 | 9 | 8 | 7:4 | 3:0 |
|-----|----------|----|----|---|---|-----|-----|
| SBZ | SD | RW | S | 0 | 0 | Domain | Status |

**SD** indicates an AXI Decode or Slave error caused the abort (only valid for 
external aborts, for all other should be zero). **RW** indicates whether a read 
(0) or a write (1) access caused the abort. The **S**-flag is part of the status 
field. The **Domain** bits indicate the domain which was accessed when the abort
occurred. The **Status** bits show the type of fault generated. See the 
[Data Fault Status Register]-manual for a list.

### Instruction Fault Status Register

This register holds the source of the last instruction fault. The bits have the following functions:

| Bit 31:13 | 12 | 11 | 10 | 9 :4 | 3:0 |
|-----|----------|----|----|------|-----|
| SBZ | SD | SBZ | 0  | SBZ | Status |

See the [Instruction Fault Status Register]-manual for the list of status combinations.

### Reading the status registers

```armasm
    data_abort:
        // save the link-register
        mov r6,lr
        // get the last executed instruction
        ldr r8,[r6,#-8]
        
        // reading the status register
        mrc p15,0,r4,c5,c0,0 ;@ data/combined
        mrc p15,0,r5,c5,c0,1 ;@ instruction
        mov sp,#0x00004000
        
        // print data fault status register
        mov r0,r4
        bl hexstring
        
        // print instruction fault status register
        mov r0,r5
        bl hexstring
        
        // print the link register
        mov r0,r6
        bl hexstring
        
        // print the bit-representation of the last executed instruction
        mov r0,r8
        bl hexstring
        
        b hang
```

Running the code results in:

```
    00045678 
    00000019 
    00000000 
    00008104 
    E5900000
```

The first line is the one correct data-read we do in our code above. The 
next value is the data fault status register, which indicates that the domain
0x01 was accessed and aborted with an 0x09 fault, i.e. a Domain Section fault. 
The third line is the value of the instruction fault status register, which
indicates a status of 0x0, i.e. "no function, reset value". That probably means 
no fault happened.

The fourth line is the link register, i.e. the address to the instruction which 
would have been executed next, and the last line is the binary representation 
of the instruction which caused the fault. Use a disassembler to view the 
instruction in all its mnemonic glory:

```
    80fc:	e5900000 	ldr	r0, [r0]
```

# Conclusion

This is just a simple intro to MMUs, just enough to be dangerous. The MMU is one
of the simplest peripherals to program so long as bit manipulations are not 
something that causes you to lose sleep. But if you mess it up even a bit, or 
forget something, you can crash in spectacular ways (often silently without any 
way of knowing what really happened. Debugging can be hard at best.

The ARM ARM indicates that ARMv6 adds a feature of separating the data from the
instructions from the MMUs perspective, which is an interesting thought (see the 
JTAG-debugging comments). 

[ARM ARM]: https://www.scss.tcd.ie/~waldroj/3d1/arm_arm.pdf
[co-processor]: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0301h/ch03s02s01.html
[Data Fault Status Register]: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0301h/Bgbiaghh.html
[Instruction Fault Status Register]: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0301h/Bgbccfgi.html
