
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

.globl _start
_start:
    b skip

.space 0x200000-0x0004,0

skip:
    mrs x0,mpidr_el1
    mov x1,#0xC1000000
    bic x0,x0,x1
    cbz x0,master
    b hang
master:

    mov sp,#0x08000000
    bl notmain
hang: b hang

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

.globl BRANCHTO
BRANCHTO:
    mov w30,w0
    ret


.globl dummy
dummy:
    ret

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


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
