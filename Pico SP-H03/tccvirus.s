; https://stackoverflow.com/questions/6139952/what-is-the-booting-process-for-arm
	.text
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu softvfp
	.type	__entry, %function
	.type	enable_watchdog, %function
	.type	gpio_identification, %function
	.type	gpio_signal, %function
	.type	watchdog_clear, %function
	.type	_delay, %function
__entry:
	bl enable_watchdog
gpio:
	bl watchdog_clear
	b gpio

.set  EN, (1<<0)
.set IEN, (1<<3)
.set TCKSEL, (1<<4)
.set WATCHDOG_EN, (1<<31)
.set WATCHDOG_CLR, (1<<30)

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

watchdog_clear:
	mov	r12, lr
	ldr	r0, .WATCHDOG_ADDR
	ldr	r1, [r0]
	orr	r1, r1, #WATCHDOG_CLR
	str	r1, [r0]
	mov lr, r12
	mov pc, lr

.CONTROL_ADDR:
	.word	0xf0404000
.TWDCFG:
	.word	0xf0403070
.TWDCLR_ADDR:
	.word	0xf0403074
.WATCHDOG_ADDR:
	.word	0xf040400c

.set CONTROL_POFF, (1<<1)

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
.set GPIO_GPxEN_OFFSET, 0x04
.set GPIO_GPxXOR_OFFSET, 0x10
.set GPIO_GPxPD0_OFFSET, 0x1c
.set GPIO_GPxPD1_OFFSET, 0x20
.set GPIO_GPxFN0_OFFSET, 0x24
.set GPIO_GPxFN1_OFFSET, 0x28
.set GPIO_GPxFN2_OFFSET, 0x2c
.set GPIO_GPxFN3_OFFSET, 0x30
gpio_signal:
	mov	r11, lr                     @ save the return pointer
	/* initialize the GPIO requested in OUTPUT mode */
	ldr	r3, .GPIO_REGISTER_MAP_ADDR
	mov r7, #GPIO_GROUP_OFFSET
	mul	r4, r8, r7
	add	r5, r3, r4                 @ r5 points at the start of the right GPIO group address
	add	r6, r5, #GPIO_GPxEN_OFFSET @ r6 points to the direction control
	mov	r7, #1
	lsl	r7, r9                     @ r7 contains the pattern for the right GPIO to be activated
	str	r7, [r6]                   @ set OUTPUT MODE
	/* set pull-down */
	add	r6, r5, #GPIO_GPxPD0_OFFSET
	ldr	r3, .GPIO_PULL_DOWN
	str	r3, [r6]
	add	r6, r5, #GPIO_GPxPD1_OFFSET
	ldr	r3, .GPIO_PULL_DOWN
	str	r3, [r6]
	/* and set function mode to 0 */
	add	r6, r5, #GPIO_GPxFN0_OFFSET
	mov	r3, #0
	str	r3, [r6]
	add	r6, r5, #GPIO_GPxFN1_OFFSET
	mov	r3, #0
	str	r3, [r6]
	add	r6, r5, #GPIO_GPxFN2_OFFSET
	mov	r3, #0
	str	r3, [r6]
	add	r6, r5, #GPIO_GPxFN3_OFFSET
	mov	r3, #0
	str	r3, [r6]

	/* from now on, we need to loop over the encoding */
	orr	r4, r9, r8, LSL #5         @ r4 contains the number of cycles to toggle the GPIO
	mov	r3, #0                     @ this will be our counter
_loop_over_encoding:
	add	r6, r5, #GPIO_GPxXOR_OFFSET @ r6 points at the GPxXOR register
	str	r7, [r6]                   @ we toggle the corresponding register
	ldr	r0, .DELAY_VALUE
	bl _delay
	str	r7, [r6]                   @ twice so to have a square wave
	bl _delay
	cmp	r3, r4
	add	r3, r3, #1
	bne	_loop_over_encoding

	/* now set to zero the GPIO */
	add	r6, r5, #GPIO_GPXCLR_OFFSET
	str	r7, [r6]

	/* return at home */
	mov	lr, r11
	mov pc, lr
.DELAY_VALUE:
	.word	0xf
.GPIO_REGISTER_MAP_ADDR:
	.word	0xf0102000
.GPIO_PULL_DOWN:
	.word	0xaaaaaaaa
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

/* takes r0 as number of loop to wait
 * internally uses r1 and r2
 */
_delay:
	mov	r2, lr
	mov	r1, #0x00
_loop_delay:
	add	r1, r1, #1
	cmp	r1, r0
	bne	_loop_delay
	mov	lr, r2
	mov	pc, lr
