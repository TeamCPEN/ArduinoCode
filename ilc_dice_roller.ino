

const int numPins = 14;             // Number of digital pins inputs x 2
int currentPinStateArray[numPins/2];  // array for state values of pins

int lastPinStateArray[numPins / 2];
char serialDataBuffer[8];  // used as a buffer array

const unsigned long commandTimeout = 3000;  // Timeout period in milliseconds
unsigned long lastCommandTime = 0;

const int switchPinArray[numPins / 2] = { 2, 3, 4, 5, 6, 7, 8 };         // New Pin Numbers
const int outputPinArray[numPins / 2] = { A0, A1, A2, A3, A4, A5, 9 };  // New Pin Numbers wired to 7 seg as a b c d e f g 
const int SwitchPowerPin = 10;                                           // new pin number
const int ClkPin = 9;

float newClockSpeed = 0.0;
bool isClockOn = false;

const byte digitPatterns[10] = {
  0b1111110, // 0
  0b0110000, // 1
  0b1101101, // 2
  0b1111001, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1110011  // 9
};

bool isDiceRollActive = false;
unsigned long lastDiceRollTime = 0;

void setup() {
  SetSwitchPinsToInput();
  SetOutputPinsToOutput();
  pinMode(ClkPin, OUTPUT);
  randomSeed(analogRead(A7));
}

void loop() {
  ReadSwitchStates();
  processSwitchStates();

  if (!isDiceRollActive || millis() - lastDiceRollTime > 3000) { // 3000 ms since last dice roll
    spinAnimation();
  }
}

void spinAnimation() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(outputPinArray[i], HIGH);
    delay(200);  // Short delay between segments
    digitalWrite(outputPinArray[i], LOW);
  }
}



void ReadSwitchStates() {

  //lastPinStateArray[] = currentPinStateArray[];  // store last state of pins

  // Read and store each state of pin into statArray(is Printed later)
  for (int i = 0; i < numPins / 2; i++) {
    currentPinStateArray[i] = digitalRead(switchPinArray[i]);
  }
}

void processSwitchStates() {
  isDiceRollActive = false;
  for (int i = 0; i < numPins / 2; i++) {
    if (currentPinStateArray[i] != lastPinStateArray[i]) {
      lastPinStateArray[i] = currentPinStateArray[i];
      if (currentPinStateArray[i] == HIGH) {
        isDiceRollActive = true;
        lastDiceRollTime = millis();
        int result;
        switch(i) {
          case 0:
            result = getRandomInt(1, 5);
            break;
          case 1:
            result = getRandomInt(1, 7);
            break;
          case 2:
            result = getRandomInt(1, 9);
            break;
          case 3:
            result = getRandomInt(1, 11);
            break;
          case 4:
            result = getRandomInt(1, 13);
            break;
          case 5:
            result = getRandomInt(1, 21);
            break;
          case 6:
            result = getRandomInt(1, 101);
            break;
        }
        setOutputPinsToNumber(result);
      }
    }
  }
}

int getRandomInt(int min, int max) {
  int randomInt = random(min, max);
  return randomInt;
}



void setOutputPinsToNumber(int num) {
  clearDisplay(); // Ensure all segments are off before displaying new number
  String numStr = String(num);
  for (int i = 0; i < numStr.length(); i++) {
    int digit = numStr.charAt(i) - '0';
    displayDigit(digit);
    delay(1500);
    clearDisplay();
  }
  digitalWrite(outputPinArray[6], HIGH);
  delay(500);
  digitalWrite(outputPinArray[6], LOW);
}

void displayDigit(int digit) {
  byte pattern = digitPatterns[digit]; // Get the pattern for the digit
  for (int i = 0; i < 7; i++) {
    digitalWrite(outputPinArray[i], bitRead(pattern, 6 - i)); // Set each segment
  }
}

void clearDisplay() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(outputPinArray[i], LOW); // Turn off all segments
    
  }
  
}



void SetSwitchPinsToInput() {  // Sets switch pins to INPUT

  for (int i = 0; i < numPins / 2; i++) {
    digitalWrite(switchPinArray[i], LOW);
    pinMode(switchPinArray[i], INPUT);
  }
  // delay(500);
  digitalWrite(SwitchPowerPin, HIGH);  // turns back on manual mode

}



void SetOutputPinsToOutput() {  //

  for (int i = 0; i < numPins / 2; i++) {
    pinMode(outputPinArray[i], OUTPUT);
    digitalWrite(outputPinArray[i], LOW);
  }
}


void resetStateArray() {
  for (int i = 0; i < numPins / 2; i++) {
    currentPinStateArray[i] = 0;
    currentPinStateArray[i + 7] = 0;
  }
}



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




