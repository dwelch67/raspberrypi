

.globl _start
_start:
    ldr pc,add_handler_00
    ldr pc,add_handler_04
    ldr pc,add_handler_08
    ldr pc,add_handler_0C
    ldr pc,add_handler_10
    ldr pc,add_handler_14
    ldr pc,add_handler_18
    ldr pc,add_handler_1C
add_handler_00: .word reset
add_handler_04: .word handler_04
add_handler_08: .word handler_08
add_handler_0C: .word handler_0C
add_handler_10: .word handler_10
add_handler_14: .word handler_14
add_handler_18: .word handler_18
add_handler_1C: .word handler_1C

reset:
    mov r0,#0x8000
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

    mov sp,#0x00100000
    mov r0,pc
    bl notmain
hang: b hang



handler_04:
    mov r0,#0x04
    b handler

handler_08:
    mov r0,#0x08
    b handler

handler_0C:
    mov r0,#0x0C
    b handler

handler_10:
    mov r7,r0
    mov r0,#0x10
    ;@b handler
    b data_abort

handler_14:
    mov r0,#0x14
    b handler

handler_18:
    mov r0,#0x18
    b handler

handler_1C:
    mov r0,#0x1C
    b handler


handler:
    mov r4,lr
    mov sp,#0x00004000
    bl hexstring
    mov r0,r4
    bl hexstring
    b hang

data_abort:
    mov r6,lr
    ldr r8,[r6,#-8]
    mrc p15,0,r4,c5,c0,0 ;@ data/combined
    mrc p15,0,r5,c5,c0,1 ;@ instruction
    mov sp,#0x00004000
    bl hexstring
    mov r0,r4
    bl hexstring
    mov r0,r5
    bl hexstring
    mov r0,r6
    bl hexstring
    mov r0,r8
    bl hexstring
    mov r0,r7
    bl hexstring
    b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr

.globl start_mmu
start_mmu:
    mov r2,#0
    mcr p15,0,r2,c7,c7,0 ;@ invalidate caches
    mcr p15,0,r2,c8,c7,0 ;@ invalidate tlb
    mcr p15,0,r2,c7,c10,4 ;@ DSB ??

    mvn r2,#0
    bic r2,#0xC
    mcr p15,0,r2,c3,c0,0 ;@ domain

    mcr p15,0,r0,c2,c0,0 ;@ tlb base
    mcr p15,0,r0,c2,c0,1 ;@ tlb base

    mrc p15,0,r2,c1,c0,0
    orr r2,r2,r1
    mcr p15,0,r2,c1,c0,0

    bx lr

.globl stop_mmu
stop_mmu:
    mrc p15,0,r2,c1,c0,0
    bic r2,#0x1000
    bic r2,#0x0004
    bic r2,#0x0001
    mcr p15,0,r2,c1,c0,0
    bx lr

.globl invalidate_tlbs
invalidate_tlbs:
    mov r2,#0
    mcr p15,0,r2,c8,c7,0  ;@ invalidate tlb
    mcr p15,0,r2,c7,c10,4 ;@ DSB ??
    bx lr



;@-------------------------------------------------------------------------
;@
;@ Copyright (c) 2012 David Welch dwelch@dwelch.com
;@
;@ Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
;@
;@ The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;@
;@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;@
;@-------------------------------------------------------------------------
