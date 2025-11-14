#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// ADC read from ADC0 (A0)
static uint16_t adc_read(void) {
    // Select ADC0, reference = AVcc (REFS0 = 1)
    ADMUX = (1 << REFS0) | (0 << MUX0); // REFS0=1, MUX=0 (ADC0)
    _delay_us(5); // small settle time

    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait until conversion finishes
    while (ADCSRA & (1 << ADSC));
    // Read ADC (10-bit)
    uint16_t val = ADC;
    return val;
}

// Map 0..1023 to 0..255 (8-bit)
static uint8_t map10to8(uint16_t v) {
    // simple integer mapping with rounding
    uint32_t tmp = (uint32_t)v * 255U;
    tmp = (tmp + 511U) / 1023U; // rounding
    return (uint8_t)tmp;
}

int main(void) {
    // ----- I/O setup -----
    // PB1 (pin 9 / OC1A) as output (PWM)
    DDRB |= (1 << DDB1);
    ADMUX  = (1 << REFS0);                 // ADC0 selected later, ref = AVcc
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12)  | (1 << CS11);

    while (1) {
        // Fan off for 15 s
        OCR1A = 0;                // duty = 0%
        // during off period, could still sample pot (not necessary)
        for (uint8_t i = 0; i < 15; i++) {
            _delay_ms(1000);
        }

        // Fan on for 5 s; update duty from pot every 200 ms
        uint16_t elapsed_ms = 0;
        while (elapsed_ms < 5000) {
            uint16_t adc = adc_read();       // 0..1023
            uint8_t pwm = map10to8(adc);     // 0..255
            OCR1A = pwm;                     // set duty (8-bit)
            _delay_ms(200);
            elapsed_ms += 200;
        }

        // ensure fan off at loop end (optional)
        OCR1A = 0;
    }

    return 0;
}
