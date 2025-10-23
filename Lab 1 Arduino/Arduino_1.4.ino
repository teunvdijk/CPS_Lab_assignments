  #include "sam.h" 
  #define LED_PIN 10  // PB10 corresponds to pin 10 in Group[1] (PORTB)

void setup() {
  // Enable the APB clock for the PORT module
  PM->APBBMASK.reg |= PM_APBBMASK_PORT;

  // Configure PB10 (D4) as OUTPUT
  PORT->Group[1].DIRSET.reg = (1 << LED_PIN);
}

void loop() {
  // Toggle LED ON
  PORT->Group[1].OUTSET.reg = (1 << LED_PIN);
  delay(1000);

  // Toggle LED OFF
  PORT->Group[1].OUTCLR.reg = (1 << LED_PIN);
  delay(1000);
}