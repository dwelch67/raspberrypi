
.globl _start
_start:
    b reset
reset:
    ldr sp,stack_start
    ldr r0,thumb_start_add
    bx r0

stack_start: .word 0x1000
thumb_start_add: .word thumb_start
.word 0
.word 0

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
