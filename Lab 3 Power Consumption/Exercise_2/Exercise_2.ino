#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED_PIN PB5   // onboard LED (Arduino D13)

ISR(TIMER1_COMPA_vect)
{
    // Wake-up: blink LED for 0.1 s
    PORTB |=  (1 << LED_PIN);   // LED ON
    _delay_ms(2000);             // 2 s delay
    PORTB &= ~(1 << LED_PIN);   // LED OFF
}

int main(void)
{
    // --- Configure LED pin as output ---
    DDRB  |= (1 << LED_PIN);
    PORTB &= ~(1 << LED_PIN);   // LED starts OFF

    // --- Timer1 setup ---
    // 16 MHz / 1024 = 15625 Hz → OCR1A = 15625 gives 1 s period
    TCCR1A = 0;                                    // normal port operation
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);  // CTC mode, prescaler = 1024
    OCR1A  = 15625*4;                                // compare value for 4 s
    TIMSK1 = (1 << OCIE1A);                        // enable Timer1 compare interrupt

    // --- Sleep setup ---
    set_sleep_mode(SLEEP_MODE_IDLE);   // Timer1 keeps running in IDLE
    sleep_enable();

    sei();   // enable global interrupts

    while (1)
    {
        sleep_cpu();   // CPU sleeps until Timer1 interrupt fires
        // after ISR finishes, MCU returns here and sleeps again
    }
}