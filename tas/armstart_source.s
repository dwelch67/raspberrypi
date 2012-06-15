
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
    b .
