

.globl _start
_start:
    b skip

.space 0x8000-0x0004,0

skip:
    mrs x0,mpidr_el1
    mov x1,#0xFF000000
    bic x0,x0,x1
    cbz x0,core_zero
    sub x1,x0,#1
    cbz x1,core_one
    sub x1,x0,#2
    cbz x1,core_two
    sub x1,x0,#3
    cbz x1,core_three

    mrs x0,mpidr_el1
    ;mov x1,#0x40000
    ;str w0,[x1]
    b hang

core_zero:
    mov sp,#0x8000
    bl notmain
hang: b hang

core_one:
    mov sp,#0x6000
    mov x1,#0
    str w1,[sp]
    mov x0,#0
core_one_loop:
    ldr w0,[sp]
    cbz w0,core_one_loop
    bl hopper
    b hang

core_two:
    mov sp,#0x4000
    mov x1,#0
    str w1,[sp]
    mov x0,#0
core_two_loop:
    ldr w0,[sp]
    cbz w0,core_two_loop
    bl hopper
    b hang

core_three:
    mov sp,#0x2000
    mov x1,#0
    str w1,[sp]
    mov x0,#0
core_three_loop:
    ldr w0,[sp]
    cbz w0,core_three_loop
    bl hopper
    b hang

hopper:
    br x0


.globl PUT32
PUT32:
  str w1,[x0]
  ret

.globl GET32
GET32:
    ldr w0,[x0]
    ret


.globl GETPC
GETPC:
    mov x0,x30
    ret

.globl dummy
dummy:
    ret

//-------------------------------------------------------------------------
//
// Copyright (c) 2016 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
