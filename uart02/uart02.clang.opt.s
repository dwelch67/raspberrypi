	.syntax unified
	.eabi_attribute 10, 2
	.fpu vfpv2
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.file	"uart02.clang.opt.bc"
	.text
	.globl	uart_putc
	.align	2
	.type	uart_putc,%function
uart_putc:                              @ @uart_putc
@ BB#0:                                 @ %entry
	push	{r4, lr}
	mov	r4, r0
.LBB0_1:                                @ %while.body
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI0_0
	bl	GET32
	tst	r0, #32
	beq	.LBB0_1
@ BB#2:                                 @ %while.end
	ldr	r0, .LCPI0_1
	mov	r1, r4
	bl	PUT32
	pop	{r4, pc}
	.align	2
@ BB#3:
.LCPI0_0:
	.long	539054164               @ 0x20215054
.LCPI0_1:
	.long	539054144               @ 0x20215040
.Ltmp0:
	.size	uart_putc, .Ltmp0-uart_putc

	.globl	hexstrings
	.align	2
	.type	hexstrings,%function
hexstrings:                             @ @hexstrings
@ BB#0:                                 @ %entry
	push	{r4, r5, r6, lr}
	mov	r4, r0
	mov	r0, #55
	add	r5, r0, r4, lsr #28
	lsr	r6, r4, #28
.LBB1_1:                                @ %while.body.i
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI1_0
	bl	GET32
	tst	r0, #32
	beq	.LBB1_1
@ BB#2:                                 @ %uart_putc.exit
	ldr	r0, .LCPI1_1
	orr	r1, r6, #48
	cmp	r4, #-1610612736
	movhs	r1, r5
	bl	PUT32
	mov	r0, #15
	and	r6, r0, r4, lsr #24
	add	r5, r6, #55
.LBB1_3:                                @ %while.body.i.1
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI1_0
	bl	GET32
	tst	r0, #32
	beq	.LBB1_3
@ BB#4:                                 @ %uart_putc.exit.1
	ldr	r0, .LCPI1_1
	orr	r1, r6, #48
	cmp	r6, #9
	movhi	r1, r5
	bl	PUT32
	mov	r0, #15
	and	r6, r0, r4, lsr #20
	add	r5, r6, #55
.LBB1_5:                                @ %while.body.i.2
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI1_0
	bl	GET32
	tst	r0, #32
	beq	.LBB1_5
@ BB#6:                                 @ %uart_putc.exit.2
	ldr	r0, .LCPI1_1
	orr	r1, r6, #48
	cmp	r6, #9
	movhi	r1, r5
	bl	PUT32
	mov	r0, #15
	and	r6, r0, r4, lsr #16
	add	r5, r6, #55
.LBB1_7:                                @ %while.body.i.3
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI1_0
	bl	GET32
	tst	r0, #32
	beq	.LBB1_7
@ BB#8:                                 @ %uart_putc.exit.3
	ldr	r0, .LCPI1_1
	orr	r1, r6, #48
	cmp	r6, #9
	movhi	r1, r5
	bl	PUT32
	mov	r0, #15
	and	r6, r0, r4, lsr #12
	add	r5, r6, #55
.LBB1_9:                                @ %while.body.i.4
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI1_0
	bl	GET32
	tst	r0, #32
	beq	.LBB1_9
@ BB#10:                                @ %uart_putc.exit.4
	ldr	r0, .LCPI1_1
	orr	r1, r6, #48
	cmp	r6, #9
	movhi	r1, r5
	bl	PUT32
	mov	r0, #15
	and	r6, r0, r4, lsr #8
	add	r5, r6, #55
.LBB1_11:                               @ %while.body.i.5
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI1_0
	bl	GET32
	tst	r0, #32
	beq	.LBB1_11
@ BB#12:                                @ %uart_putc.exit.5
	ldr	r0, .LCPI1_1
	orr	r1, r6, #48
	cmp	r6, #9
	movhi	r1, r5
	bl	PUT32
	mov	r0, #15
	and	r6, r0, r4, lsr #4
	add	r5, r6, #55
.LBB1_13:                               @ %while.body.i.6
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI1_0
	bl	GET32
	tst	r0, #32
	beq	.LBB1_13
@ BB#14:                                @ %uart_putc.exit.6
	ldr	r0, .LCPI1_1
	orr	r1, r6, #48
	cmp	r6, #9
	movhi	r1, r5
	bl	PUT32
	and	r5, r4, #15
	add	r4, r5, #55
.LBB1_15:                               @ %while.body.i.7
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI1_0
	bl	GET32
	tst	r0, #32
	beq	.LBB1_15
@ BB#16:                                @ %uart_putc.exit.7
	ldr	r0, .LCPI1_1
	orr	r1, r5, #48
	cmp	r5, #9
	movhi	r1, r4
	bl	PUT32
.LBB1_17:                               @ %while.body.i4
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI1_0
	bl	GET32
	tst	r0, #32
	beq	.LBB1_17
@ BB#18:                                @ %uart_putc.exit5
	ldr	r0, .LCPI1_1
	mov	r1, #32
	bl	PUT32
	pop	{r4, r5, r6, pc}
	.align	2
@ BB#19:
.LCPI1_0:
	.long	539054164               @ 0x20215054
.LCPI1_1:
	.long	539054144               @ 0x20215040
.Ltmp1:
	.size	hexstrings, .Ltmp1-hexstrings

	.globl	hexstring
	.align	2
	.type	hexstring,%function
hexstring:                              @ @hexstring
@ BB#0:                                 @ %entry
	push	{lr}
	bl	hexstrings
.LBB2_1:                                @ %while.body.i
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI2_0
	bl	GET32
	tst	r0, #32
	beq	.LBB2_1
@ BB#2:                                 @ %uart_putc.exit
	ldr	r0, .LCPI2_1
	mov	r1, #13
	bl	PUT32
.LBB2_3:                                @ %while.body.i4
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI2_0
	bl	GET32
	tst	r0, #32
	beq	.LBB2_3
@ BB#4:                                 @ %uart_putc.exit5
	ldr	r0, .LCPI2_1
	mov	r1, #10
	bl	PUT32
	pop	{lr}
	bx	lr
	.align	2
@ BB#5:
.LCPI2_0:
	.long	539054164               @ 0x20215054
.LCPI2_1:
	.long	539054144               @ 0x20215040
.Ltmp2:
	.size	hexstring, .Ltmp2-hexstring

	.globl	notmain
	.align	2
	.type	notmain,%function
notmain:                                @ @notmain
@ BB#0:                                 @ %entry
	push	{r4, r5, r6, lr}
	mov	r4, r0
	ldr	r0, .LCPI3_0
	mov	r1, #1
	bl	PUT32
	ldr	r6, .LCPI3_1
	mov	r1, #0
	mov	r5, #0
	mov	r0, r6
	bl	PUT32
	ldr	r0, .LCPI3_2
	mov	r1, #0
	bl	PUT32
	ldr	r0, .LCPI3_3
	mov	r1, #3
	bl	PUT32
	ldr	r0, .LCPI3_4
	mov	r1, #0
	bl	PUT32
	mov	r0, r6
	mov	r1, #0
	bl	PUT32
	ldr	r0, .LCPI3_5
	mov	r1, #198
	bl	PUT32
	ldr	r0, .LCPI3_6
	mov	r1, #14
	orr	r1, r1, #256
	bl	PUT32
	ldr	r6, .LCPI3_7
	mov	r0, r6
	bl	GET32
	bic	r0, r0, #258048
	orr	r1, r0, #73728
	mov	r0, r6
	bl	PUT32
	ldr	r0, .LCPI3_8
	mov	r1, #0
	bl	PUT32
.LBB3_1:                                @ %for.body
                                        @ =>This Inner Loop Header: Depth=1
	mov	r0, r5
	bl	dummy
	add	r5, r5, #1
	cmp	r5, #150
	bne	.LBB3_1
@ BB#2:                                 @ %for.end
	ldr	r0, .LCPI3_9
	mov	r1, #49152
	bl	PUT32
	mov	r5, #0
.LBB3_3:                                @ %for.body5
                                        @ =>This Inner Loop Header: Depth=1
	mov	r0, r5
	bl	dummy
	add	r5, r5, #1
	cmp	r5, #150
	bne	.LBB3_3
@ BB#4:                                 @ %for.end8
	ldr	r0, .LCPI3_9
	mov	r1, #0
	bl	PUT32
	ldr	r0, .LCPI3_2
	mov	r1, #3
	bl	PUT32
	ldr	r0, .LCPI3_10
	bl	hexstrings
.LBB3_5:                                @ %while.body.i.i
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI3_11
	bl	GET32
	tst	r0, #32
	beq	.LBB3_5
@ BB#6:                                 @ %uart_putc.exit.i
	ldr	r0, .LCPI3_12
	mov	r1, #13
	bl	PUT32
.LBB3_7:                                @ %while.body.i4.i
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI3_11
	bl	GET32
	tst	r0, #32
	beq	.LBB3_7
@ BB#8:                                 @ %hexstring.exit
	ldr	r0, .LCPI3_12
	mov	r1, #10
	bl	PUT32
	mov	r0, r4
	bl	hexstrings
.LBB3_9:                                @ %while.body.i.i5
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI3_11
	bl	GET32
	tst	r0, #32
	beq	.LBB3_9
@ BB#10:                                @ %uart_putc.exit.i6
	ldr	r0, .LCPI3_12
	mov	r1, #13
	bl	PUT32
.LBB3_11:                               @ %while.body.i4.i10
                                        @ =>This Inner Loop Header: Depth=1
	ldr	r0, .LCPI3_11
	bl	GET32
	tst	r0, #32
	beq	.LBB3_11
@ BB#12:                                @ %hexstring.exit11
	ldr	r0, .LCPI3_12
	mov	r1, #10
	b	.LBB3_14
.LBB3_13:                               @ %while.end
                                        @   in Loop: Header=BB3_14 Depth=1
	ldr	r4, .LCPI3_12
	mov	r0, r4
	bl	GET32
	mov	r1, r0
	mov	r0, r4
.LBB3_14:                               @ %hexstring.exit11
                                        @ =>This Loop Header: Depth=1
                                        @     Child Loop BB3_15 Depth 2
	bl	PUT32
.LBB3_15:                               @ %while.body10
                                        @   Parent Loop BB3_14 Depth=1
                                        @ =>  This Inner Loop Header: Depth=2
	ldr	r0, .LCPI3_11
	bl	GET32
	tst	r0, #1
	beq	.LBB3_15
	b	.LBB3_13
	.align	2
@ BB#16:
.LCPI3_0:
	.long	539054084               @ 0x20215004
.LCPI3_1:
	.long	539054148               @ 0x20215044
.LCPI3_2:
	.long	539054176               @ 0x20215060
.LCPI3_3:
	.long	539054156               @ 0x2021504c
.LCPI3_4:
	.long	539054160               @ 0x20215050
.LCPI3_5:
	.long	539054152               @ 0x20215048
.LCPI3_6:
	.long	539054184               @ 0x20215068
.LCPI3_7:
	.long	538968068               @ 0x20200004
.LCPI3_8:
	.long	538968212               @ 0x20200094
.LCPI3_9:
	.long	538968216               @ 0x20200098
.LCPI3_10:
	.long	305419896               @ 0x12345678
.LCPI3_11:
	.long	539054164               @ 0x20215054
.LCPI3_12:
	.long	539054144               @ 0x20215040
.Ltmp3:
	.size	notmain, .Ltmp3-notmain


