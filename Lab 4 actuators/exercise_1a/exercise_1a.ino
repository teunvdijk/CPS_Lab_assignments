#define F_CPU 16000000UL      
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
   
    DDRB |= (1 << DDB1);      // Stel PB1 (pin 9) in als output

    while (1) {
        // LED aan
        PORTB |= (1 << PORTB1);
        _delay_ms(3000);     

        // LED uit
        PORTB &= ~(1 << PORTB1);
        _delay_ms(1000);      
    }

    return 0;
}