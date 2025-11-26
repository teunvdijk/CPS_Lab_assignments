#include <SPI.h>

#define SS_PIN 8   // Chip Select pin

void setup() {
  Serial.begin(9600);

  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH); // slave niet geselecteerd

  // Start SPI met de juiste instellingen
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  digitalWrite(SS_PIN, LOW); // selecteer slave
  byte received = SPI.transfer(0x55); // stuur 0x55, ontvang tegelijk een byte
  digitalWrite(SS_PIN, HIGH); // deselecteer slave

  Serial.print("Ontvangen: ");
  Serial.println(received, HEX);

  delay(1000);
}