
;@ ------------------------------------------------------------------
;@ ------------------------------------------------------------------


.globl _start
_start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,hyp_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word smc
prefetch_handler:   .word hang
data_handler:       .word hang
hyp_handler:        .word hang
irq_handler:        .word hang
fiq_handler:        .word hang

reset:

    ;@ b skip
    mrs r0,cpsr
    bic r0,r0,#0x1F
    orr r0,r0,#0x13
    msr spsr_cxsf,r0
    add r0,pc,#4
    msr ELR_hyp,r0
    eret
skip:

    mrc p15, 0, r1, c12, c0, 0 ;@ get vbar
    mov r0,#0x8000
    ;@ mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

    mov r12,#0
    mcr p15, 0, r12, c7, c10, 1
    dsb
    mov r12, #0
    mcr p15, 0, r12, c7, c5, 0
    mov r12, #0
    mcr p15, 0, r12, c7, c5, 6
    dsb
    isb
    smc #0

    mrc p15,0,r2,c1,c0,0
    bic r2,#0x1000
    bic r2,#0x0004
    mcr p15,0,r2,c1,c0,0

    mov sp,#0x8000
    mov r0,pc
    bl notmain
hang: b hang

smc:
    mrc p15, 0, r1, c1, c1, 0
    bic r1, r1, #1
    mcr p15, 0, r1, c1, c1, 0
    movs    pc, lr

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

;@ ------------------------------------------------------------------
;@ ------------------------------------------------------------------


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
