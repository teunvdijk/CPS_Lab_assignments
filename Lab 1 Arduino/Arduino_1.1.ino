const int ledPin = 6;       // LED on D6
const int potPin = A1;      // Potentiometer on A1

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);       // Start serial communication
}

void loop() {
    // Blink LED ON
  digitalWrite(ledPin, HIGH);
  delay(500);

  // Blink LED OFF
  digitalWrite(ledPin, LOW);
  delay(500);

  // Read and print potentiometer value once per second
  int potValue = analogRead(potPin);  // 0–4095 on MKR1010
  Serial.println(potValue);
}
