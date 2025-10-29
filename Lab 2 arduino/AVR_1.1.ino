const int buttonPin = 2;   // INT0
const int ledPin = 13;     // Built-in LED
volatile bool ledState = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Button active LOW
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleLED, FALLING);
}

void loop() {
  // Nothing here — LED toggling is handled by interrupt
}

void toggleLED() {
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
}
