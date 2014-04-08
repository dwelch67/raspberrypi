
.device ATmega168

.equ  DDRB       = 0x04
.equ  PORTB      = 0x05
.equ  TCCR0A     = 0x24
.equ  TCCR0B     = 0x25
.equ  TCNT0      = 0x26

;.equ  CLKPR      = 0x61

.org 0x0000
    rjmp RESET

RESET:

;    ldi r16,0x80
;    ldi r17,0x00
;    sts CLKPR,r16
;    sts CLKPR,r17


    ldi R16,0x00
    out TCCR0A,R16
    ldi R16,0x05
    out TCCR0B,R16

    ldi R16,0x01
    out DDRB,R16

    ldi R17,0x00
    ldi R20,0x01
Loop:


    ldi R18,0xF0 ;; change this to change the blink rate
aloop:
    in R17,TCNT0
    cpi R17,0x00
    brne aloop

bloop:
    in R17,TCNT0
    cpi R17,0x00
    breq bloop

    inc R18
    cpi R18,0x00
    brne aloop

    eor R16,R20
    out PORTB, R16
    rjmp Loop
