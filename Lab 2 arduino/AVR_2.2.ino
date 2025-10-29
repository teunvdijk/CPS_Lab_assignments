#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN     PB5    // D13
#define SWITCH_PIN  PB0    // D8

volatile uint8_t ledState = 0;

ISR(PCINT0_vect) {
    if (!(PINB & (1 << SWITCH_PIN))) {
        PINB |= (1 << LED_PIN);  // toggle LED directly
    }
}

int main(void) {
    // --- LED setup
    DDRB |= (1 << LED_PIN);              // D13 output

    // --- Switch setup
    DDRB &= ~(1 << SWITCH_PIN);          // D8 input
    PORTB |= (1 << SWITCH_PIN);          // enable pull-up

    // --- Pin-change interrupt setup
    PCICR |= (1 << PCIE0);               // enable PORTB pin-change group
    PCMSK0 |= (1 << PCINT0);             // unmask PB0 (pin 8)
    PCIFR |= (1 << PCIF0);               // clear any pending flag
    sei();                               // enable global interrupts
    while (1) {
        // nothing here; LED handled in ISR
    }
}
