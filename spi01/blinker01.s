.device ATmega168
.equ  DDRB       = 0x04
.equ  PORTB      = 0x05

.org 0x0000
    rjmp RESET

RESET:
    ldi R16,0x01
    out DDRB,R16

    ldi R18,0x00
    ldi R17,0x00
    ldi R20,0x01
Loop:

    ldi R19,0xE8
aloop:
    inc R17
    cpi R17,0x00
    brne aloop

    inc R18
    cpi R18,0x00
    brne aloop

    inc R19
    cpi R19,0x00
    brne aloop

    eor R16,R20
    out PORTB, R16
    rjmp Loop

