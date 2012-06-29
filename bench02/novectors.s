
;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------

.code 32

.globl _start
_start:
    mov sp,#0x8000
    bl notmain
    b .

.globl start_l1cache
start_l1cache:
    mov r0, #0
    mcr p15, 0, r0, c7, c7, 0 ;@ invalidate caches
    mcr p15, 0, r0, c8, c7, 0 ;@ invalidate tlb
    mrc p15, 0, r0, c1, c0, 0
    orr r0,r0,#0x1000 ;@ instruction
    orr r0,r0,#0x0004 ;@ data
    mcr p15, 0, r0, c1, c0, 0
    bx lr

.globl stop_l1cache
stop_l1cache:
    mrc p15, 0, r0, c1, c0, 0
    bic r0,r0,#0x1000 ;@ instruction
    bic r0,r0,#0x0004 ;@ data
    mcr p15, 0, r0, c1, c0, 0
    bx lr

    nop

.globl ARMTEST0
ARMTEST0:
    subs r0,r0,#1
    bne ARMTEST0
    bx lr

    nop

.globl ARMTEST1
ARMTEST1:
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    bne ARMTEST1
    bx lr

.globl ARMTEST2
ARMTEST2:
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    subs r0,r0,#1
    bne ARMTEST2
    bx lr

.globl ARMTEST3
ARMTEST3:
    subs r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    subs r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    subs r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    subs r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    subs r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    subs r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    subs r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    subs r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    bne ARMTEST3
    bx lr

;@ -----------------------------------------------------

.thumb

.thumb_func
thumb_start:
    bl notmain
hang: b hang

.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.thumb_func
.globl dummy
dummy:
    bx lr

.thumb_func
.globl THUMBTEST0
THUMBTEST0:
    sub r0,r0,#1
    bne THUMBTEST0
    bx lr


.thumb_func
.globl THUMBTEST1
THUMBTEST1:
    sub r0,r0,#1
    sub r0,r0,#1
    sub r0,r0,#1
    sub r0,r0,#1
    sub r0,r0,#1
    sub r0,r0,#1
    sub r0,r0,#1
    sub r0,r0,#1
    bne THUMBTEST1
    bx lr


.thumb_func
.globl THUMBTEST2
THUMBTEST2:
    sub r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    sub r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    sub r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    sub r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    sub r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    sub r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    sub r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    sub r0,r0,#1
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    bne THUMBTEST2
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

