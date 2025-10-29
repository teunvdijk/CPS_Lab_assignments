#include <avr/io.h>
#include <avr/interrupt.h>

void setup_timer1_ctc() {
    // Set CTC mode
    TCCR1B |= (1 << WGM12);

    // Set compare value for 1Hz (1 second)
    OCR1A = 15625;

    // Enable Timer1 Compare Match A interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Set prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

int main(void) {
    // Set PB5 (pin 13) as output
    DDRB |= (1 << DDB5);

    // Setup Timer1
    setup_timer1_ctc();

    // Enable global interrupts
    sei();

    while (1) {
        // Main loop does nothing, LED toggling is handled in ISR
    }
}

// Interrupt Service Routine for Timer1 Compare Match A
ISR(TIMER1_COMPA_vect) {
    PORTB ^= (1 << PORTB5); // Toggle LED
}
