
; 00000000 <_start>:
;    0: eaffffff    b   4 <reset>
;
; 00000004 <reset>:
;    4: e59fd004    ldr sp, [pc, #4]    ; 10 <stack_start>
;    8: e59f0004    ldr r0, [pc, #4]    ; 14 <thumb_start_add>
;    c: e12fff10    bx  r0
;
; 00000010 <stack_start>:
;   10: 08000000
;
; 00000014 <thumb_start_add>:
;   14: 00000021
;   18: 00000000
;   1C: 00000000
;
; 00000020 <thumb_start>:
;
;0x00000000
.hword 0xFFFF
.hword 0xEAFF
;0x00000004
.hword 0xD004
.hword 0xE59F
;0x00000008
.hword 0x0004
.hword 0xE59F
;0x0000000C
.hword 0xFF10
.hword 0xE12F
;0x00000010
.hword 0x0000
.hword 0x0800
;0x00000014
.hword 0x8021
.hword 0x0000
;0x00000018
.hword 0x0000
.hword 0x0000
;0x0000001C
.hword 0x0000
.hword 0x0000
;0x00000020


