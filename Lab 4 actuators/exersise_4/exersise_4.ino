#define LED_PIN 9  // Use Arduino pin 9

void setup() {
  pinMode(LED_PIN, OUTPUT);  // Set pin 9 as output
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // Turn LED ON
  delay(3000);                   // 3 seconds

  digitalWrite(LED_PIN, LOW);   // Turn LED OFF
  delay(10000);                  // 10 seconds
}