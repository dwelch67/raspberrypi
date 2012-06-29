	.syntax unified
	.eabi_attribute 10, 2
	.fpu vfpv2
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.file	"blinker03.clang.opt.bc"
	.text
	.globl	notmain
	.align	2
	.type	notmain,%function
notmain:                                @ @notmain
@ BB#0:                                 @ %entry
	push	{r4, r5, r6, r7, lr}
	ldr	r4, .LCPI0_0
	mov	r0, r4
	bl	GET32
	bic	r0, r0, #1835008
	orr	r1, r0, #262144
	mov	r0, r4
	bl	PUT32
	ldr	r4, .LCPI0_1
	mov	r1, #16318464
	mov	r0, r4
	bl	PUT32
	mov	r1, #512
	orr	r1, r1, #16318464
	mov	r0, r4
	bl	PUT32
	ldr	r0, .LCPI0_2
	bl	GET32
	mov	r5, #2304
	mov	r6, #4608
	mov	r4, r0
	orr	r5, r5, #3997696
	orr	r6, r6, #7995392
	b	.LBB0_2
.LBB0_1:                                @ %while.end12
                                        @   in Loop: Header=BB0_2 Depth=1
	add	r4, r4, r6
.LBB0_2:                                @ %while.body
                                        @ =>This Loop Header: Depth=1
                                        @     Child Loop BB0_5 Depth 2
                                        @     Child Loop BB0_3 Depth 2
	ldr	r0, .LCPI0_3
	mov	r1, #65536
	bl	PUT32
.LBB0_3:                                @ %while.body3
                                        @   Parent Loop BB0_2 Depth=1
                                        @ =>  This Inner Loop Header: Depth=2
	ldr	r0, .LCPI0_2
	bl	GET32
	sub	r0, r0, r4
	cmp	r0, r5
	blo	.LBB0_3
@ BB#4:                                 @ %while.end
                                        @   in Loop: Header=BB0_2 Depth=1
	ldr	r0, .LCPI0_4
	mov	r1, #65536
	bl	PUT32
	ldr	r0, .LCPI0_5
	sub	r7, r0, r4
.LBB0_5:                                @ %while.body6
                                        @   Parent Loop BB0_2 Depth=1
                                        @ =>  This Inner Loop Header: Depth=2
	ldr	r0, .LCPI0_2
	bl	GET32
	add	r0, r7, r0
	cmp	r0, r5
	blo	.LBB0_5
	b	.LBB0_1
	.align	2
@ BB#6:
.LCPI0_0:
	.long	538968068               @ 0x20200004
.LCPI0_1:
	.long	536917000               @ 0x2000b408
.LCPI0_2:
	.long	536917024               @ 0x2000b420
.LCPI0_3:
	.long	538968092               @ 0x2020001c
.LCPI0_4:
	.long	538968104               @ 0x20200028
.LCPI0_5:
	.long	4290967296              @ 0xffc2f700
.Ltmp0:
	.size	notmain, .Ltmp0-notmain


