#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LED_PIN PB5

// Start with LED OFF, so first period = 0.9 s
volatile uint8_t led_on = 0;

ISR(TIMER1_COMPA_vect)
{
    if (led_on)
    {
        // LED currently ON → turn OFF
        PORTB &= ~(1 << LED_PIN);
        OCR1A = 14063;   // 0.9 s delay (15625 * 0.9)
        led_on = 0;
    }
    else
    {
        // LED currently OFF → turn ON
        PORTB |= (1 << LED_PIN);
        OCR1A = 1563;    // 0.1 s delay (15625 * 0.1)
        led_on = 1;
    }
}

int main(void)
{
    // --- Configure LED pin as output ---
    DDRB |= (1 << LED_PIN);
    PORTB &= ~(1 << LED_PIN);

    // --- Timer1 setup ---
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);  // CTC mode, prescaler = 1024
    OCR1A  = 14063;             // Start with LED off → 0.9 s period
    TIMSK1 = (1 << OCIE1A);     // Enable compare match interrupt

    // --- Sleep mode setup ---
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    sei();   // Enable global interrupts

    while (1)
    {
        sleep_cpu();   // Sleep between timer interrupts
    }
}
