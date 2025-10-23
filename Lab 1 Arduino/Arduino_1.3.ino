void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial Monitor to open
  Serial.println("Simple Calculator Ready!");
  Serial.println("Enter an expression like: 8 * 5");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // remove spaces, tabs, \r, etc.

    // Echo the input back to the terminal
    Serial.print("You entered: ");
    Serial.println(input);
    
    // Find the operator
    int opIndex = input.indexOf('+');
    if (opIndex < 0) opIndex = input.indexOf('-');
    if (opIndex < 0) opIndex = input.indexOf('*');
    if (opIndex < 0) opIndex = input.indexOf('/');

    if (opIndex > 0) {
      // Split into two parts
      String left = input.substring(0, opIndex);
      String right = input.substring(opIndex + 1);

      float num1 = left.toFloat();
      float num2 = right.toFloat();
      char op = input.charAt(opIndex);

      float result;
      bool valid = true;

      switch (op) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
          if (num2 != 0) result = num1 / num2;
          else { Serial.println("Error: divide by zero"); valid = false; }
          break;
        default: valid = false;
      }

      if (valid) {
        Serial.print("Result: ");
        Serial.println(result);
      }
    } else {
      Serial.println("Invalid input. Use e.g. 8+5 or 8 * 5");
    }
  }
}
