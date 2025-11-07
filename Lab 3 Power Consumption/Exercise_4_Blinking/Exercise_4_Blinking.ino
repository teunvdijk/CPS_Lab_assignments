#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED_PIN PB5     // Onboard LED (D13)
#define SWITCH_PIN PD2  // INT0-pin (D2)

ISR(INT0_vect)
{
  // MCU wakes up → blink LED briefly
  PORTB |=  (1 << LED_PIN);   // LED aan
  _delay_ms(100);             // 0.1 s delay
  PORTB &= ~(1 << LED_PIN);   // LED uit
}

void setup() {
  // --- LED ---
  DDRB  |=  (1 << LED_PIN);
  PORTB &= ~(1 << LED_PIN);

  // --- Switch (PD2 / INT0) ---
  DDRD  &= ~(1 << SWITCH_PIN);   // Input
  PORTD |=  (1 << SWITCH_PIN);   // Enable internal pull-up

  // --- Externe interrupt (INT0) ---
  EICRA |=  (1 << ISC01);  // Falling edge triggers interrupt
  EICRA &= ~(1 << ISC00);
  EIMSK |=  (1 << INT0);   // Enable INT0 interrupt

  // --- Power reduction (bespaar energie) ---
  PRR = (1 << PRTWI) | (1 << PRTIM0) | (1 << PRTIM1) |
        (1 << PRTIM2) | (1 << PRSPI) | (1 << PRADC);

  // --- Sleep mode setup ---
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Deep sleep
  sleep_enable();

  sei();  // Enable global interrupts
}

void loop() {
  sleep_cpu();  // MCU slaapt tot switch een interrupt veroorzaakt
  // Na ISR gaat hij terug naar deze loop → slaapt weer verder
}