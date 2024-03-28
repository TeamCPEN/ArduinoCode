void setClk(float freq) {
  TCCR1A = 0x41;                // Change to use COM1A1:0 bits for OCR1A (pin 9)
  TCCR1B = 0x14;                // Same as before, as we are still using Timer 1
  OCR1A = 0x7A12 / (freq * 2);  // Set frequency for pin 9
  OCR1B = OCR1A * 0.5;          // This line now affects pin 10 instead of pin 9
  isClockOn = true;
}

void offClk() {

  TCCR1A = 0x00;   // Reset TCCR1A to default
  TCCR1B = 0x00;   // Reset TCCR1B to default
  OCR1A = 0x0000;  // Reset OCR1A to default
  OCR1B = 0x0000;  // Reset OCR1B to default
  delay(10);
  digitalWrite(ClkPin, LOW);
  isClockOn = false;
}

void toggleClk() {
  if (isClockOn == false) {
    delay(5);
    digitalWrite(ClkPin, HIGH);
    delay(5);
    digitalWrite(ClkPin, LOW);
  }
}

void cycleAndState() {
  toggleClk();
  delay(5);
  RequestStateLog();
}