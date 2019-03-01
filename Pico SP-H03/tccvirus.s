; https://stackoverflow.com/questions/6139952/what-is-the-booting-process-for-arm
        .cpu arm7tdmi
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"tccvirus.c"
	.text
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu softvfp
	.type	main, %function
main:
	ldr	r3, .L3
	ldrb	r4, [r3]
	orr	r4, r4, #1
	strb	r4, [r3]
	ldrb	r4, [r3]
	bic	r4, r4, #48
	orr	r4, r4, #64
	strb	r4, [r3]
.loop:
	b .loop
.L4:
	.align	2
.L3:
	.word	0xf0403070
	.size	main, .-main
	.ident	"GCC: (15:6.3.1+svn253039-1build1) 6.3.1 20170620"
