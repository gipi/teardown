/*
 * Proof of concept for the firmware that handles the ADFU board reset:
 * the code activates by an external interrupt and reset the board and
 * hold high the CE signal of the flash so to force the ADFU mode.
 *
 * These two signals must be delayed one from the other, the phases
 * are the following
 *
 *  1. the signals are floating
 *  2. the reset is set in output mode with a low level
 *  3. the ce signal is set to high
 *  4. the reset signal is set floating
 *  5. a couple of seconds
 *  6. the ce is set floating
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PIN_LED               PB5
#define PIN_EXTERNAL_TRIGGER  PD2

#define PIN_RESET             PD3
#define PIN_FLASH_CE          PD4

#define TIME_MS_LED           1000
#define TIME_MS_HOLD_CE      2000
#define TIME_MS_HOLD_RESET    100

struct pin_t {
    uint8_t* portx;
    uint8_t* ddrx;
    uint8_t  x;
};

/* this define a variable named pin_name */
#define pin(name,group,n) \
    struct pin_t pin_##name = {    \
        .portx = &PORT##group,     \
        .ddrx = &DDR##group,       \
        .x = P##group##n,          \
    }

inline void pin_set_mode_ouput(struct pin_t pin) {
    *pin.ddrx |= _BV(pin.x);
}

inline void pin_set_mode_input(struct pin_t pin) {
    *pin.ddrx &= ~_BV(pin.x);
}

inline void pin_set_low(struct pin_t pin) {
    pin_set_mode_ouput(pin);
    *pin.portx &= ~_BV(pin.x);
}

inline void pin_set_high(struct pin_t pin) {
    pin_set_mode_ouput(pin);
    *pin.portx |= _BV(pin.x);
}

inline void pin_set_hiz(struct pin_t pin) {
    pin_set_mode_input(pin);
    *pin.portx &= ~_BV(pin.x);
}

inline void pin_enable_pullup(struct pin_t pin) {
    pin_set_mode_input(pin);
    *pin.portx |= _BV(pin.x);
}

pin(led,B,5);
pin(flash_ce,D,4);
pin(am7331_reset,D,3);
pin(trigger,D,2);

void heartbeat();
void setup();

volatile unsigned int g_is_adfu_time = 0;

/* the trigger is via the PD2 gpio */
ISR(INT0_vect) {
    /* set the global variable to not overload the interrupt routine */
    g_is_adfu_time = 1;
}

inline void led_on() {
    pin_set_high(pin_led);
}
/*
 * show that the code is running
 */
void heartbeat() {
    _delay_ms(TIME_MS_LED);
    PORTB &= ~(1 << PIN_LED); //Turn LED off
    _delay_ms(TIME_MS_LED);
    led_on();
    /* the led remains on so that we can see when the code is running something else */
}

void setup_led() {
    pin_set_mode_ouput(pin_led);
    led_on();
}

void reset_low() {
    pin_set_low(pin_am7331_reset);
}

void reset_high() {
    pin_set_high(pin_am7331_reset);
}

void reset_floating() {
    pin_set_hiz(pin_am7331_reset);
}

void ce_disable() {
    pin_set_high(pin_flash_ce);
}

void ce_floating() {
    pin_set_hiz(pin_flash_ce);
}

void reset_board() {
    reset_low();
    _delay_ms(TIME_MS_HOLD_RESET);
    reset_floating();
}

void adfu_mode() {
    reset_low();
    ce_disable();
    _delay_ms(TIME_MS_HOLD_RESET);
    reset_floating();
    _delay_ms(TIME_MS_HOLD_CE);
    ce_floating();
}


void setup_adfu_pins() {
    reset_floating();
    ce_floating();
}

void setup_trigger() {
    pin_set_mode_input(pin_trigger);
    pin_enable_pullup(pin_trigger);

    EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
    EIMSK |= (1 << INT0);     // Turns on INT0

    sei();                    // turn on interrupts
}

void setup() {
    setup_led();
    setup_adfu_pins();
    setup_trigger();
}

int main(void) {
    reset_board();
    setup();
    while(1) {
        heartbeat();
        if (g_is_adfu_time) {
            adfu_mode();
            g_is_adfu_time = 0;
        }
    }
}
