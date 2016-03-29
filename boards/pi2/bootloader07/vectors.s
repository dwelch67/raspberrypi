
;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------

.globl _start
_start:
    b skip

.space 0x200000-0x8004,0

skip:

    ;@ stop caching !!!
    ;@ mrc p15,0,r2,c1,c0,0
    ;@ bic r2,#0x1000
    ;@ bic r2,#0x0004
    ;@ mcr p15,0,r2,c1,c0,0

    mov sp,#0x08000000
    bl notmain
hang: b hang

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
