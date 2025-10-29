const int switchPin = 8;   // PB0
const int ledPin    = 13;  // PB5
volatile bool ledState = false;
volatile bool lastSwitchState = HIGH;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);   // use internal pull-up
}

void loop() {
  bool current = digitalRead(switchPin);
  if (lastSwitchState == HIGH && current == LOW) {
    // Falling edge detected
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
  lastSwitchState = current;
}