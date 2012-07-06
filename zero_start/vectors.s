
.globl _start
_start:
    b reset
    b reset
    b reset
    b reset

    b reset
    b reset
    b reset
    b reset

text_size_x:   .word text_size
soffset: .word reset - _start + 8

reset:
    mov r0,pc
    ldr r1,soffset
    sub r0,r1
    cmp r0,#0
    beq skip_copy
    mov r1,#0;
    ldr r2,text_size_x
    add r2,r2,#0xFF
    add r2,r2,#15
    mov r2,r2,lsr #4
copy_loop:
    ldmia r0!,{r4,r5,r6,r7}
    stmia r1!,{r4,r5,r6,r7}
    subs r2,r2,#1
    bne copy_loop
    mov r0,#0
    bx r0

skip_copy:

    mov sp,#0x8000
    mov r0,pc
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

