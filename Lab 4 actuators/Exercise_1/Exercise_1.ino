#define F_CPU 16000000UL     // 16 MHz system clock
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // --- Configure PB1 (Arduino digital pin 9) as output ---
    DDRB |= (1 << PB1);      // Set bit 1 of DDRB to 1 → output

    while (1)
    {
        // LED ON
        PORTB |= (1 << PB1);   // Set bit 1 → logic high
        _delay_ms(3000);       // Wait 3 seconds

        // LED OFF
        PORTB &= ~(1 << PB1);  // Clear bit 1 → logic low
        _delay_ms(1000);       // Wait 1 second
    }
}

