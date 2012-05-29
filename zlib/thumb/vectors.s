
.globl _start
_start:
    b reset
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang

reset:
    mov sp,#0x200000
    bl thumb_start
hang: b hang

.thumb
.thumb_func
thumb_start:
    ldr r0,=notmain
    bx r0


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

