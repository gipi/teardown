; https://stackoverflow.com/questions/6139952/what-is-the-booting-process-for-arm
	.text
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu softvfp
	.type	main, %function
.set  EN, (1<<0)
.set IEN, (1<<3)
.set TCKSEL, (1<<4)
.set WATCHDOG_EN, (1<<31)

main:
	ldr	r3, .TWDCFG
	ldrb	r4, [r3]
	orr	r4, r4, #(EN | IEN)
	strb	r4, [r3]
	ldrb	r4, [r3]
	bic	r4, r4, 0x30 @ clear the TCKSEL
	orr	r4, r4, 0x40 @ set the TCKSEL to 0x04
	strb	r4, [r3]
	ldr	r3, .WATCHDOG_ADDR
	ldr	r4, [r3]
	orr	r4, r4, #WATCHDOG_EN
	str	r4, [r3]
.loop:
	b .loop
.TWDCFG:
	.word	0xf0403070
.WATCHDOG_ADDR:
	.word	0xf040400c
