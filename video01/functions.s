	
.section .data
.align 1
foreColor:
.hword 0xFFFF

.align 2
graphicsAddress:
.int 0

.align 4
font:
	.incbin "font1.font"

.section .text
.globl SetForeColor
SetForeColor:
cmp r0,#0x10000
movhs pc,lr
ldr r1,=foreColor
strh r0,[r1]
mov pc,lr

.globl SetGraphicsAddress
SetGraphicsAddress:
ldr r1,=graphicsAddress
str r0,[r1]
mov pc,lr

.globl DrawPixel
DrawPixel:
	px .req r0
	py .req r1
	addr .req r2
	ldr addr,=graphicsAddress
	ldr addr,[addr]
	height .req r3
	ldr height,[addr,#4]
	sub height,#1
	cmp py,height
	movhi pc,lr
	.unreq height

	width .req r3
	ldr width,[addr,#0]
	sub width,#1
	cmp px,width
	movhi pc,lr
	ldr addr,[addr,#32]
	add width,#1
	mla px,py,width,px
	.unreq width
	.unreq py
	add addr, px,lsl #1
	.unreq px
	fore .req r3
	ldr fore,=foreColor
	ldrh fore,[fore]
	strh fore,[addr]
	.unreq fore
	.unreq addr
	mov pc,lr

.globl Random	@r0 last random #
Random:
	xnm .req r0
	a .req r1
	ldr a,=0xef35
	mul a,xnm
	mul a,xnm
	add a, xnm
	.unreq xnm
	add r0,a,#73
	.unreq a
	mov pc,lr
	
.globl DrawChar
DrawChar:
	cmp r0,#127
	movhi r0,#0
	movhi r1,#0
	movhi pc,lr

	push {r4,r5,r6,r7,r8,lr}
	x .req r4
	y .req r5
	charAddr .req r6
	mov x,r1
	mov y,r2
	ldr charAddr,=font
	add charAddr, r0,lsl #4

	lineLoop$:
		bits .req r7
		bit .req r8
		ldrb bits,[charAddr]
		mov bit,#8

		charPixelLoop$:
			subs bit,#1
			blt charPixelLoopEnd$
			lsl bits,#1
			tst bits,#0x100
			beq charPixelLoop$
			ldr r2,=0xffff
			add r0,x,bit
			mov r1,y
			bl setPixel

			teq bit,#0
			bne charPixelLoop$
		charPixelLoopEnd$:
			.unreq bit
			.unreq bits
			add y,#1
			add charAddr,#1
			tst charAddr,#0b1111
			bne lineLoop$
	.unreq x
	.unreq y
	.unreq charAddr

	width .req r0
	height .req r1
	mov width,#8
	mov height,#16

	pop {r4,r5,r6,r7,r8,pc}
	.unreq width
	.unreq height
