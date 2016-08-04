
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


.globl BRANCHTO
BRANCHTO:
    bx r0

.globl ASMDELAY
ASMDELAY:
    subs r0,r0,#1
    bne ASMDELAY
    bx lr
    bne ASMDELAY
    bne ASMDELAY
    bne ASMDELAY
    nop
    

.globl HOP
HOP:
    bx r1

.globl GET_CONTROL
GET_CONTROL:
    MRC p15,0,r0,c1,c0,0
    bx lr

.globl SET_CONTROL
SET_CONTROL:
    MRC p15,0,r1,c1,c0,0
    orr r1,r0
    mov r0,#0
    MCR p15, 0, r0, c7, c10, 4 ;@ gross overkill
    MCR p15, 0, r0, c7, c10, 5 ;@ gross overkill
    MCR p15, 0, r0, c7, c5, 4  ;@ gross overkill
    MCR p15, 0, r0, c7, c5, 6  ;@ gross overkill
    MCR p15,0,r1,c1,c0,0
    MCR p15, 0, r0, c7, c10, 4 ;@ gross overkill
    MCR p15, 0, r0, c7, c10, 5 ;@ gross overkill
    MCR p15, 0, r0, c7, c5, 4  ;@ gross overkill
    MCR p15, 0, r0, c7, c5, 6  ;@ gross overkill
    bx lr

.globl CLR_CONTROL
CLR_CONTROL:
    MRC p15,0,r1,c1,c0,0
    bic r1,r0
    MCR p15,0,r1,c1,c0,0
    bx lr


.globl PrefetchFlush
PrefetchFlush:
    MCR p15, 0, r0, c7, c5, 4 
    MCR p15, 0, r0, c7, c5, 6 
    bx lr
    
.globl start_l1cache
start_l1cache:
    mov r0, #0
    mcr p15, 0, r0, c7, c7, 0 ;@ invalidate caches
    mcr p15, 0, r0, c8, c7, 0 ;@ invalidate tlb
    MCR p15, 0, r0, c7, c10, 4 ;@ DSB needed?
    MCR p15, 0, r0, c7, c10, 5 ;@ DMB needed?
    mrc p15, 0, r0, c1, c0, 0
    orr r0,r0,#0x1000
    mcr p15, 0, r0, c1, c0, 0
    bx lr

.globl stop_l1cache
stop_l1cache:
    mrc p15, 0, r0, c1, c0, 0
    bic r0,r0,#0x1000
    mcr p15, 0, r0, c1, c0, 0
    bx lr

    MCR p15, 0, r0, c7, c10, 4 ;@ DSB
    MCR p15, 0, r0, c7, c10, 5 ;@ DMB


.globl invalidate_l1cache
invalidate_l1cache:
    mov r0, #0
    mcr p15, 0, r0, c7, c7, 0 ;@ invalidate caches
    mcr p15, 0, r0, c8, c7, 0 ;@ invalidate tlb

    MCR p15, 0, r0, c7, c5, 4  ;@ gross overkill
    MCR p15, 0, r0, c7, c5, 6  ;@ gross overkill

    
    MCR p15, 0, r0, c7, c10, 4 ;@ DSB needed?
    MCR p15, 0, r0, c7, c10, 5 ;@ DMB needed?
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
