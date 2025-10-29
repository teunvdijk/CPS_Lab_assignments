#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN PB5
#define SWITCH_PIN PD2

volatile uint8_t ledState = 0;

ISR(INT0_vect) {
    ledState ^= 1;                 // Toggle LED state
    if (ledState)
        PORTB |= (1 << PORTB5);    // Turn LED on (D13)
    else
        PORTB &= ~(1 << PORTB5);   // Turn LED off
}

int main(void) {
    DDRB |= (1 << DDB5);           // Set pin 13 as output
    DDRD &= ~(1 << DDD2);          // Set pin D2 (INT0) as input
    PORTD |= (1 << PORTD2);        // Enable pull-up resistor on D2

    // Configure INT0 to trigger on falling edge:
    EICRA |= (1 << ISC01);         // ISC01=1, ISC00=0 → falling edge
    EICRA &= ~(1 << ISC00);

    EIMSK |= (1 << INT0);          // Enable external interrupt INT0
    sei();                         // Enable global interrupts
    while (1) {
        // Main loop does nothing
    }
}
