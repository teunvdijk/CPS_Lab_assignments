#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // --- PWM op PB1 / OC1A configureren ---
    DDRB |= (1 << DDB1);          // Pin 9 als output

    // Fast PWM, 8-bit, non-inverting op OC1A
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12)  | (1 << CS11);  // prescaler = 8 → ~7.8 kHz PWM-frequentie

    while (1) {
        // 15 s wachten (fan uit)
        OCR1A = 0;                // duty = 0 %
        _delay_ms(5000);

        // Fan 5 s aan, snelheid stijgt elke seconde
        for (uint8_t i = 0; i <= 5; i++) {
            OCR1A = i * 50;       // duty cycle ↑: 0, 50, 100, 150, 200, 250 (≈ 0–100 %)
            _delay_ms(1000);      // 1 s per stap
        }

        OCR1A = 0;                // fan uit na 5 s
    }
}