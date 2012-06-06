
.code 32

.globl _start
_start:
    b reset
reset:
    ldr sp,stack_start
    ldr r0,thumb_start_add
    bx r0

stack_start: .word 0x10000
thumb_start_add: .word thumb_start

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


