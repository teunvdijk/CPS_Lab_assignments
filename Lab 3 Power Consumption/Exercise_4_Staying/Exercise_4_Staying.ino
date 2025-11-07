#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED_PIN PB5
#define SWITCH_PIN PD2  // INT0 (D2)

ISR(INT0_vect)
{
  // Niks doen in de interrupt zelf — enkel wakker worden
}

void setup() {
  DDRB  |=  (1 << LED_PIN);     // LED output
  PORTB &= ~(1 << LED_PIN);

  DDRD  &= ~(1 << SWITCH_PIN);  // Switch input
  PORTD |=  (1 << SWITCH_PIN);  // Enable internal pull-up

  // Falling edge interrupt → wekt MCU
  EICRA |=  (1 << ISC01);
  EICRA &= ~(1 << ISC00);
  EIMSK |=  (1 << INT0);

  // Power reduction
  PRR = (1 << PRTWI) | (1 << PRTIM0) | (1 << PRTIM1) |
        (1 << PRTIM2) | (1 << PRSPI) | (1 << PRADC);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sei();
}

void loop() {
  // Lees de huidige stand van de switch
  if (PIND & (1 << SWITCH_PIN)) {
    // Hoog → schakelaar open → LED uit
    PORTB &= ~(1 << LED_PIN);
  } else {
    // Laag → schakelaar gesloten → LED aan
    PORTB |= (1 << LED_PIN);
  }

  // Ga terug slapen
  sleep_cpu();
}