
;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------


.globl _start
_start:
    b reset
    b undef
    bl other
    bl other
    bl other
    bl other
    bl other
    bl other
    bl other
    bl other
    bl other
    bl other
    bl other
    bl other
    bl other
    bl other

reset:

    ;@ enable fpu
    mrc p15, 0, r0, c1, c0, 2
    orr r0,r0,#0x300000 ;@ single precision
    orr r0,r0,#0xC00000 ;@ double precision
    mcr p15, 0, r0, c1, c0, 2
    mov r0,#0x40000000
    fmxr fpexc,r0

    ;@ (PSR_UND_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xDB
    msr cpsr_c,r0
    mov sp,#0x00100000

    ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD3
    msr cpsr_c,r0

    mov sp,#0x00200000
    bl notmain
hang: b hang

other:
    mov sp,#0x00100000
    push {lr}
    ldr r0,=0xBADBAD
    bl hexstring
    pop {r0}
    bl hexstring
    b hang

undef:
    push {r0}
    mov r0,#0x40000000
    fmxr fpexc,r0
    pop {r0}
    subs pc,lr,#4


;@undef:
    mov sp,#0x00100000
    ldr r1,[lr,#-4]
    push {r1}
    ldr r0,=0xBAD
    bl hexstring
    pop {r0}
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


.globl m4add
m4add:
    vmov s0,r0
    vmov s1,r1
    vmov s2,r2
    vmov s3,r3
    vadd.f32 s4,s0,s1
    vadd.f32 s5,s2,s3
    vmov r0,s5
    bx lr

.globl m4add2
m4add2:
    vmov s0,r0
    vmov s1,r1
    vmov s2,r2
    vmov s3,r3
    vadd.f32 s4,s0,s1
    vadd.f32 s5,s2,s3
    vmov r0,s4
    bx lr

.globl m4vmov
m4vmov:
    vmov s4,r0
    vmov s5,r0
    bx lr



;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------


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
