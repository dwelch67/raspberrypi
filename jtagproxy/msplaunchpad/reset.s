
.global reset
reset:

  mov #0x03FF,r1
  call #notmain
  jmp hang

.global hang
hang:
  jmp hang

.global asmdelay
asmdelay:
    dec r15
    jnz asmdelay
    ret


