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
.set CONTROL_POFF, (1<<1)
main:
	bl gpio_identification

enable_watchdog:
	mov	r10, lr
	ldr	r3, .TWDCFG
	ldrb	r4, [r3]
	orr	r4, r4, #(EN | IEN)
	strb	r4, [r3]
	ldrb	r4, [r3]
	bic	r4, r4, 0x30 @ clear the TCKSEL
	orr	r4, r4, 0x40 @ set the TCKSEL to 0x04
	strb	r4, [r3]
	ldr	r3, .TWDCLR_ADDR
	mov r4, #0x00000000
	str	r4, [r3]
	ldr	r3, .WATCHDOG_ADDR
	ldr	r4, [r3]
	orr	r4, r4, #WATCHDOG_EN
	str	r4, [r3]
	mov	lr, r10
	mov pc, lr

.CONTROL_ADDR:
	.word	0xf0404000
.TWDCFG:
	.word	0xf0403070
.TWDCLR_ADDR:
	.word	0xf0403074
.WATCHDOG_ADDR:
	.word	0xf040400c
power_off:
	ldr	r3, .CONTROL_ADDR
	ldr	r4, [r3]
	orr	r4, r4, #CONTROL_POFF
	str	r4, [r3]
/*
 * This function outputs via the GPIO, passed as the couple (group, idx)
 * its encoded identifier as a 32 bit number.
 *
 *  r8: group in [0, 5]
 *  r9: idx in [0, 31]
 *
 * so we need a number of bits to encode 6*32 = 192 values, i.e. 8bit are enough,
 * but we can use all 32-bit so that we have a start/stop pattern clearly
 * identifiable.
 *
 */
.set GPIO_GROUP_OFFSET, 0x40
.set GPIO_GPXDAT_OFFSET, 0x00
.set GPIO_GPXCLR_OFFSET, 0x0c
.set GPIO_GPXEN_OFFSET, 0x04
.set GPIO_GPXXOR_OFFSET, 0x10

gpio_signal:
	mov	r11, lr                     @ save the return pointer
	/* initialize the GPIO requested in OUTPUT mode */
	ldr	r3, .GPIO_REGISTER_MAP_ADDR
	mov r7, #GPIO_GROUP_OFFSET
	mul	r4, r8, r7
	add	r5, r3, r4                 @ r5 points at the start of the right GPIO group address
	add	r6, r5, #GPIO_GPXEN_OFFSET @ r6 points to the direction control
	mov	r7, #1
	lsl	r7, r9                     @ r7 contains the pattern for the right GPIO to be activated
	str	r7, [r6]                   @ set OUTPUT MODE

	/* from now on, we need to loop over the encoding */
	mul	r4, r8, r9                 @ r4 contains the number of cycles to toggle the GPIO
	mov	r3, #0                     @ this will be our counter
_loop_over_encoding:
	add	r6, r5, #GPIO_GPXEN_OFFSET @ r6 points at the GPxXOR register
	str	r7, [r6]                   @ we toggle the corresponding register
	str	r7, [r6]                   @ twice so to have a square wave
	add	r3, r3, #1
	cmp	r3, r4
	bne	_loop_over_encoding

	/* now set to zero the GPIO */
	add	r6, r5, #GPIO_GPXCLR_OFFSET
	str	r7, [r6]

	/* return at home */
	mov	lr, r11
	mov pc, lr
.GPIO_REGISTER_MAP_ADDR:
	.word	0xf0102000

.set GPIO_GROUPS_N, 0x06
.set GPIO_IDX_N,    0x20

gpio_identification:
	mov	r10, lr

	mov	r8, #0
_loop_group:
	mov	r9, #0
_loop_idx:
	bl	gpio_signal
	add	r9, r9, #1
	cmp	r9, #GPIO_IDX_N
	bne	_loop_idx
	add	r8, r8, #1
	cmp	r8, #GPIO_GROUPS_N
	bne _loop_group

	mov lr, r10
	mov pc, lr

