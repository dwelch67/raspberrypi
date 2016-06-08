
;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------

.globl _start
_start:
    b skip

.space 0x1000-0x4,0

skip:
    mrc p15,0,r0,c0,c0,5 ;@ MPIDR
    mov r1,#0xFF
    ands r1,r1,r0
    bne not_zero

    mov sp,#0x8000
    bl notmain
hang: b hang

not_zero:
    cmp r1,#1
    beq core_one
    cmp r1,#2
    beq core_two
    cmp r1,#3
    beq core_three
    b .

core_one:
ldr r1,=0x1000
str r0,[r1]
    mov sp,#0x6000
    mov r1,#0
    str r1,[sp]
core_one_loop:
    ldr r0,[sp]
    cmp r0,#0
    beq core_one_loop
    bl hopper
    b hang

core_two:
ldr r1,=0x1004
str r0,[r1]
    mov sp,#0x4000
    mov r1,#0
    str r1,[sp]
core_two_loop:
    ldr r0,[sp]
    cmp r0,#0
    beq core_two_loop
    bl hopper
    b hang

core_three:
ldr r1,=0x1008
str r0,[r1]
    mov sp,#0x2000
    mov r1,#0
    str r1,[sp]
core_three_loop:
    ldr r0,[sp]
    cmp r0,#0
    beq core_three_loop
    bl hopper
    b hang

hopper:
    bx r0


.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl GETPC
GETPC:
    mov r0,lr
    bx lr

.globl BRANCHTO
BRANCHTO:
    mov r12,#0
    mcr p15, 0, r12, c7, c10, 1
    dsb
    mov r12, #0
    mcr p15, 0, r12, c7, c5, 0
    mov r12, #0
    mcr p15, 0, r12, c7, c5, 6
    dsb
    isb
    bx r0

.globl dummy
dummy:
    bx lr

.globl GETCPSR
GETCPSR:
    mrs r0,cpsr
    bx lr

.globl GETSCTLR
GETSCTLR:
    mrc p15,0,r0,c1,c0,0
    bx lr

.globl GETMPIDR
GETMPIDR:
    mrc p15,0,r0,c0,c0,5 ;@ MPIDR
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
