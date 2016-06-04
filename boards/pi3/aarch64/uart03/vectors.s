

.globl _start
_start:
    mrs x0,mpidr_el1
    mov x1,#0xFF000000
    bic x0,x0,x1
    cbz x0,zero
    sub x1,x0,#1
    cbz x1,one
    sub x1,x0,#2
    cbz x1,two
    sub x1,x0,#3
    cbz x1,three

    mrs x0,mpidr_el1
    mov x1,#0x40000
    str w0,[x1]
    b hang

zero:
    mov sp,#0x8000
    bl notmain
hang: b hang

one:
    mrs x0,mpidr_el1
    mov x1,#0x10000
    str w0,[x1]
    b hang

two:
    mrs x0,mpidr_el1
    mov x1,#0x20000
    str w0,[x1]
    b hang

three:
    mrs x0,mpidr_el1
    mov x1,#0x30000
    str w0,[x1]
    b hang


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
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
