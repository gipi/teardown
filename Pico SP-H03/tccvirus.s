; https://stackoverflow.com/questions/6139952/what-is-the-booting-process-for-arm
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
.L3:
	.word	0xf0403070
