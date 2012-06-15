
.include armstart.s

;configure gpio as an output
    ldr r0,GPFSEL1
    ldr r1,[r0]

    ;ra&=~(7<<18);  zero bits 18,19,20
    mov r2,#0x7
    lsl r2,r2,#18
    bic r1,r2
    ;ra|=1<<18;     set bit 18
    mov r2,#0x1
    lsl r2,r2,#18
    orr r1,r2
    str r1,[r0]

    ;prepare registers for loop
    ldr r0,GPSET0
    ldr r1,GPCLR0
    mov r2,#1
    lsl r3,r2,#16   ;(1<<16)
    lsl r4,r2,#24   ;start count for counting loop
top:
    str r3,[r0]

    mov r5,r4
on:
    sub r5,#1
    bne on

    str r3,[r1]

    mov r5,r4
off:
    sub r5,#1
    bne off

    b top

.align
GPFSEL1: .word 0x20200004
GPSET0:  .word 0x2020001C
GPCLR0:  .word 0x20200028
