
;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------

.globl _start
_start:
    mov sp,#0x8000
    bl notmain
hang: b hang

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

.globl GETPC
GETPC:
    mov r0,lr
    bx lr


.globl start1
start1:
    ldr r0,=start_cpu1
    mov r1,#0x40000000
    str r0,[r1,#0x9C]
    bx lr

start_cpu1:
    mrc p15, 0, r0, c0, c0, 5
    mov r1,#0x40
    str r0,[r1]
    b start_cpu1

.globl start2
start2:
    ldr r0,=start_cpu2
    mov r1,#0x40000000
    str r0,[r1,#0xAC]
    bx lr

start_cpu2:
    mrc p15, 0, r0, c0, c0, 5
    mov r1,#0x40
    str r0,[r1]
    b start_cpu2

.globl start3
start3:
    ldr r0,=start_cpu3
    mov r1,#0x40000000
    str r0,[r1,#0xBC]
    bx lr

start_cpu3:
    mrc p15, 0, r0, c0, c0, 5
    mov r1,#0x40
    str r0,[r1]
    b start_cpu3










.globl BRANCHTO
BRANCHTO:
    bx r0


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
