
;@ ------------------------------------------------------------------
;@ ------------------------------------------------------------------

.globl _start
_start:
    mov r0,#0x3F000000
    orr r0,r0,#0x00200000
    ldr r1,[r0,#0x10]
    bic r1,r1,#0x00E00000
    orr r1,r1,#0x00200000
    str r1,[r0,#0x10]
    ldr r1,[r0,#0x0C]
    bic r1,r1,#0x00038000
    orr r1,r1,#0x00008000
    str r1,[r0,#0x0C]
    mov r2,#0x00008000
    mov r3,#0x00000008

mainloop:
    str r2,[r0,#0x20]
    str r3,[r0,#0x2C]
    mov r4,#0x100000
one:
    subs r4,r4,#1
    bne one
    str r2,[r0,#0x2C]
    str r3,[r0,#0x20]
    mov r4,#0x100000
two:
    subs r4,r4,#1
    bne two
    b mainloop

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
