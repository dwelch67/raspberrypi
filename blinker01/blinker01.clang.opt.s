	.syntax unified
	.cpu	arm1176jzf-s
	.eabi_attribute	6, 6
	.eabi_attribute	8, 1
	.fpu	vfpv2
	.eabi_attribute	20, 1
	.eabi_attribute	21, 1
	.eabi_attribute	23, 3
	.eabi_attribute	24, 1
	.eabi_attribute	25, 1
	.file	"blinker01.clang.opt.bc"
	.text
	.globl	notmain
	.align	2
	.type	notmain,%function
notmain:                                @ @notmain
@ BB#0:                                 @ %entry
	push	{r4, r11, lr}
	ldr	r4, .LCPI0_0
	add	r11, sp, #4
	mov	r0, r4
	bl	GET32
	bic	r0, r0, #1835008
	orr	r1, r0, #262144
	mov	r0, r4
	bl	PUT32
.LBB0_1:                                @ %while.body
                                        @ =>This Loop Header: Depth=1
                                        @     Child Loop BB0_2 Depth 2
                                        @     Child Loop BB0_4 Depth 2
	ldr	r0, .LCPI0_1
	mov	r1, #65536
	bl	PUT32
	mov	r4, #0
.LBB0_2:                                @ %for.body
                                        @   Parent Loop BB0_1 Depth=1
                                        @ =>  This Inner Loop Header: Depth=2
	mov	r0, r4
	bl	dummy
	add	r4, r4, #1
	cmp	r4, #1048576
	bne	.LBB0_2
@ BB#3:                                 @ %for.end
                                        @   in Loop: Header=BB0_1 Depth=1
	ldr	r0, .LCPI0_2
	mov	r1, #65536
	bl	PUT32
	mov	r4, #0
.LBB0_4:                                @ %for.body3
                                        @   Parent Loop BB0_1 Depth=1
                                        @ =>  This Inner Loop Header: Depth=2
	mov	r0, r4
	bl	dummy
	add	r4, r4, #1
	cmp	r4, #1048576
	bne	.LBB0_4
	b	.LBB0_1
	.align	2
@ BB#5:
.LCPI0_0:
	.long	538968068               @ 0x20200004
.LCPI0_1:
	.long	538968092               @ 0x2020001c
.LCPI0_2:
	.long	538968104               @ 0x20200028
.Ltmp0:
	.size	notmain, .Ltmp0-notmain


	.ident	"clang version 3.4 (branches/release_34 201060)"
