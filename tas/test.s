.include armstart.s
    mov r7,#0
top:
    mov r0,r7
    bl hexstring
    add r7,#1
    b top
.include hexstring.s
