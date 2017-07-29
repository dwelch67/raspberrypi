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
    MMUTABLEBASE = 0x00004000
    virtual address = 0x12345678
    -> first 12 bit (moved to right): 0x00000123
    descriptor for the section: 0x000448c
    let's assume the descriptor was 0xABC00002
    -> physical address 0xABC45678
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
     * \param[in] vadd the virtual address (only top 12 bits used)
     * \param[in] padd the physical address (only top 12 bits used)
     * \param[in] flags the flags for the section
     **/
    uint32_t mmu_section ( uint32_t virtual, uint32_t physical, uint32_t flags )
    {
        uint32_t offset = virtual >> 20;
        // plus and or are the same thing here, as MMUTABLEBASE is 14 bit aligned
        uint32_t* entry = MMUTABLEBASE | (offset<<2);
        
        // mask lower 20 bits of physical address then ORR flags and 0x02 for 1 MiB
        physval = (physval & 0xfff00000) | (flags & 0x7ffa) | 0x02; 

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
  - bit 11 (Z) enabled branch prediction
  - bit 12 (I) enables instruction cache
  - bit 22 (U) enabled non-aligned data access as well as mixed big-/little-endian data access

So simple example code implementing the the MMU-enabling process might be this:

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
    mcr p15,0,r0,c2,c0,0 ;@ tlb base
    
    // set the bits mentioned above
    ldr r1, =0x00401805
    mrc p15,0,r2,c1,c0,0
    orr r2,r2,r1
    mcr p15,0,r2,c1,c0,0
    
    mov pc, lr
```

// TODO

I am going to mess with the translation tables after the MMU is started
so the easiest way to deal with the TLB cache is to invalidate it, but
dont need to mess with main L1 cache.  ARMv6 introduces a feature to
help with this, but going with this solution.

.globl invalidate_tlbs
invalidate_tlbs:
    mov r2,#0
    mcr p15,0,r2,c8,c7,0  ;@ invalidate tlb
    mcr p15,0,r2,c7,c10,4 ;@ DSB ??
    bx lr

Something to note here.  Debugging using the JTAG based on chip debugger
makes life easier, that removing sd cards or the old days pulling an
eeprom out and putting it it in an eraser then a programmer.  BUT,
it is not completely without issue.  When and where and if you hit this
depends heavily on the core you are using and the jtag tools and the
commands you remember/prefer.  The basic problem is caches can and
often do separate instruction I fetches from data D reads and writes.
So if you have test run A of a program that has executed the instruction
at address 0xD000.  So that instruction is in the I cache.  You have
also executed the instruction at 0xC000 but it has been evicted, but
you dont actually know what is in the I cache or not, shouldnt even
try to assume.  You stop the processor, you write a new program to
memory, now these are data D writes, and go through the D cache.  Then
you set the start address and run again.  Now there are a number of
combinations here and only one if them works, the rest can lead to
failure.

For each instruction/address in the program, if the prior instruction
at that address was in the i cache, and since data writes do not go
through the i cache then the new instruction for that address is either
in the d cache or in main ram.  When you run the new program you will
get the stale/old instruction from a prior run when you fetch that
address (unless an invalidate happens, if a flush happens then you
write back, but why would an I cache flush?), and if the new instruction
at that address is not the same as the old one unpredictable results
will occur.  You can start to see the combinations, did the data
write go through to d cache or to ram, will it flush to ram and is the
i cache invalid for that address, etc.

There is also the quesiton of are the I and D caches shared, they can
be but that is both specific to the core and your setup.  Also does
the jtag debugger have the ability to disable the caches, has it done
it for you, can you do it manually.

Any time you are using the i or d caches you need to be careful using
a jtag debugger or even a bootloader type approach depending on its
design as you might end up doing data writes of instructions and going
around the i cache or worse.  So for this kind of work using a chip
reset and non volitle rom/flash based bootloader can/will save you
a lot of headaches.  If you know your debugger is solving this for you,
great, but always make sure as you change from the raspi 2 back to
a raspi 1 for example it might not be doing it and it will drive you
nuts when you keep downloading a new program and it either crashes
in a strange way or simply just keeps running the old program and
not appearing to take your new changes.

So the example is going to start with the mmu off and write to
addresses in four different 1MB address spaces.  So that later we
can play with the section descriptors and demonstrate virtual to
physical address conversion.

So write some stuff and print it out on the uart.

    PUT32(0x00045678,0x00045678);
    PUT32(0x00145678,0x00145678);
    PUT32(0x00245678,0x00245678);
    PUT32(0x00345678,0x00345678);

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

then setup the mmu with at least those four sections and the peripherals

    mmu_section(0x00000000,0x00000000,0x0000|8|4);
    mmu_section(0x00100000,0x00100000,0x0000);
    mmu_section(0x00200000,0x00200000,0x0000);
    mmu_section(0x00300000,0x00300000,0x0000);
    //peripherals
    mmu_section(0x20000000,0x20000000,0x0000); //NOT CACHED!
    mmu_section(0x20200000,0x20200000,0x0000); //NOT CACHED!

and start the mmu with the I and D caches enabled

    start_mmu(MMUTABLEBASE,0x00000001|0x1000|0x0004);

then if we read those four addresses again we get the same output
as before since we maped virtual = physical.

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

but what if we swizzle things around.  make virtual 0x001xxxxx =
physical 0x003xxxxx.  0x002 looks at 0x000 and 0x003 looks at 0x001
(dont mess with the 0x00000000 section, that is where our program is
running)

    mmu_section(0x00100000,0x00300000,0x0000);
    mmu_section(0x00200000,0x00000000,0x0000);
    mmu_section(0x00300000,0x00100000,0x0000);

and maybe we dont need to do this but do it anyway just in case

    invalidate_tlbs();

read them again.

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

the 0x000xxxxx entry was not modifed so we get 000045678 as the output
but the 0x001xxxxx read is now coming from physical 0x003xxxxx so we
get the 00345678 output, 0x002xxxxx comes from the 0x000xxxxx space
so that read gives 00045678 and the 0x003xxxxx is mapped to 0x001xxxxx
physical giving 00145678 as the output.

So up to this point the output looks like this.

DEADBEEF
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

first blob is without the mmu enabled, second with the mmu but
virtual = physical, third we use the mmu to show virtual != physical
for some ranges.

Now for some small pages, I made this function to help out.

unsigned int mmu_small ( unsigned int vadd, unsigned int padd, unsigned int flags, unsigned int mmubase )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    ra=vadd>>20;
    rb=MMUTABLEBASE|(ra<<2);
    rc=(mmubase&0xFFFFFC00)/*|(domain<<5)*/|1;
    //hexstrings(rb); hexstring(rc);
    PUT32(rb,rc); //first level descriptor
    ra=(vadd>>12)&0xFF;
    rb=(mmubase&0xFFFFFC00)|(ra<<2);
    rc=(padd&0xFFFFF000)|(0xFF0)|flags|2;
    //hexstrings(rb); hexstring(rc);
    PUT32(rb,rc); //second level descriptor
    return(0);
}

So before turning on the mmu some physical addresses were written
with some data.  The function takes the virtual, physical, flags and
where you want the secondary table to be.  Remember secondary tables
can be up to 1K in size and are aligned on a 1K boundary.


    mmu_small(0x0AA45000,0x00145000,0,0x00000400);
    mmu_small(0x0BB45000,0x00245000,0,0x00000800);
    mmu_small(0x0CC45000,0x00345000,0,0x00000C00);
    mmu_small(0x0DD45000,0x00345000,0,0x00001000);
    mmu_small(0x0DD46000,0x00146000,0,0x00001000);
    //put these back
    mmu_section(0x00100000,0x00100000,0x0000);
    mmu_section(0x00200000,0x00200000,0x0000);
    mmu_section(0x00300000,0x00300000,0x0000);
    invalidate_tlbs();

Now why did I use different secondary table addresses most of the
time but not all of the time?  A secondary table lookup is the same
first level descriptor for the top 12 bits of the address, if the
top 12 bits of the address are different it is a different secondary
table.  So to demonstrate that we actually have separation within a
section I have two small pages within a 1MB section that I point
at two different physical address spaces.  So in short if the top
12 bits of the virtual address are the same then they share the same
coarse page table, the way the function works it writes both first
and second level descriptors so if you were to do this

    mmu_small(0x0DD45000,0x00345000,0,0x00001000);
    mmu_small(0x0DD46000,0x00146000,0,0x00001400);

Then both of those virtual addresses would go to the 0x1400 table, and
the first virtual address would not have a secondary entry its
secondary entry would be in a table at 0x1000 but the first level
no longer points to 0x1000 so the mmu would get whatever it finds
in the 0x1400 table.    


The last example is just demonstrating an access violation.  Changing
the domain to that one domain we did not set full access to

    //access violation.

    mmu_section(0x00100000,0x00100000,0x0020);
    invalidate_tlbs();

    hexstring(GET32(0x00045678));
    hexstring(GET32(0x00145678));
    hexstring(GET32(0x00245678));
    hexstring(GET32(0x00345678));
    uart_send(0x0D); uart_send(0x0A);

The first 0x45678 read comes from that first level descriptor, with
that domain

00045678
00000010

How do I know what that means with that output.  Well from my blinker07
example we touched on exceptions (interrupts).  I made a generic test
fixture such that anything other than a reset prints something out
and then hangs.   In no way shape or form is this a complete handler
but what it does show is that it is the exception that is at address
0x00000010 that gets hit which is data abort.  So figuring out it was
a data abort (pretty much expected) have that then read the data fault
status registers, being a data access we expect the data/combined one
to show somthing and the instruction one to not.  Adding that
instrumentation resulted in.

00045678
00000010
00000019
00000000
00008110
E5900000
00145678

Now I switched to the ARM1176JZF-S Technical Reference Manual for more
detail and that shows the 0x01 was domain 1, the domain we used for
that access. then the 0x9 means Domain Section Fault.

The lr during the abort shows us the instruction, which you would need
to disassemble to figure out the address, or at least that is one
way to do it perhaps there is a status register for that.

The instruction and the address match our expectations for this fault.

This is simply a basic intro.  Just enough to be dangerous.  The MMU
is one of the simplest peripherals to program so long as bit
manipulation is not something that causes you to lose sleep.  What makes
it hard is that if you mess up even one bit, or forget even one thing
you can crash in spectacular ways (often silently without any way of
knowing what happened).  Debugging can be hard at best.

The ARM ARM indicates that the ARMv6 adds the feature of separating
the I and D from an mmu perspective which is an interesting thought
(see the jtag debugging comments, and think about how this can affect
you re-loading a program into ram and running) you have enough ammo
to try that.  The ARMv7 doesnt seem to have a legacy mode yet, still
reading, the descriptors and how they are addresses looks basically
the same but this code doesnt yet work on the raspi 2, so I will
continue to work on that and update this repo when I figure it out.

## Coarse paging (TODO)

The coarse_translation.ps file I have included in this repo starts
off the same way as a section, has to the logic doesnt know what
you want until it sees the first level descriptor.  If it sees a
0b01 as the lower 2 bits of the first level descriptor then this is
a coarse page table entry and it needs to do a second level fetch.
The second level fetch does not use the mmu tlb table base address
bits 31:10 of the second level address plus bits 19:12 of the
virtual address (times 4) are where the second level descriptor lives.
Note that is 8 more bits so the section is divided into 256 parts, this
page table address is similar to the mmu table address, but it needs
to be aligned on a 1K boundry (lower 10 bits zeros) and can be worst
case 1KBytes in size.

The second level descriptor format defined in the ARM ARM (small pages
are most interesting here, subpages enabled) is a little different
than a first level section, we had a domain in the first level
descriptor to get here, but now have direct access to four sets of
AP bits you/I would have to read more to know what the difference
is between the domain defined AP and these additional four, for now
I dont care this is bare metal, set them to full access (0b11) and
move on (see below about domain and ap bits).

So lets take the virtual address 0x12345678 and the MMUTABLEBASE of
0x4000 again.  The first level descriptor address is the top three
bits of the virtual address 0x123, times 4, added to the MMUTABLEBASE
0x448C.  But this time when we look it up we find a value in the
table that has the lower two bits being 0b01.  Just to be crazy lets
say that descriptor was 0xABCDE001  (ignoring the domain and other
bits just talking address right now).  That means we take 0xABCDE000
the picture shows bits 19:12 (0x45) of the virtual address (0x12345678)
so the address to the second level descriptor in this crazy case is
0xABCDE000+(0x45<<2) = 0xABCDE114  why is that crazy?  because I
chose an address where we in theory dont have ram on the raspberry pi
maybe a mirrored address space, but a sane address would have been
somewhere close to the MMUTABLEBASE so we can keep the whole of the
mmu tables in a confined area.  Used this address simply for
demonstration purposes not based on a workable solution.

### Raspberry Pi 2

If you are on the raspi 2 with multiple arm cores and are using
the multiple arm cores you need to do more reading if you want one
core to talk to another by sharing some of the memory between
them.  Same problem as peripherals basically with multiple masters
of the ram/peripheral on the far side of my cache, how do I insure
what is in my cache maches the far side?  Easiest way is to not
cache that space.  You need to read up on if the cores share a cache
or have their own (or if l2 if present is shared but l1 is not),
ldrex/strex were implemented specifically for multi core, but you
need to understand the cache effects on these instructions (<grin>
not documented well, I have an example on just this one topic).


[ARM ARM]: https://www.scss.tcd.ie/~waldroj/3d1/arm_arm.pdf
[co-processor]: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0301h/index.html
