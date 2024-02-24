
// version number 5 goes with ILC software v7.2

const int numPins = 14;                   // Number of digital pins inputs x 2 
int currentPinStateArray[numPins];        // array for state values of pins
int lastPinStateArray[numPins/2];
char serialDataBuffer[8];                 //used as a buffer array

const unsigned long commandTimeout = 5000; // Timeout period in milliseconds
unsigned long lastCommandTime = 0;

const int switchPinArray[numPins/2] = {2,3,4,5,6,7,8};         //New Pin Numbers
const int outputPinArray[numPins/2] = {13,14,15,16,17,18,19};  //New Pin Numbers
const int SwitchPowerPin = 10;                                //new pin number
const int ClkPin = 9;

unsigned long startTime = 0.0;  //used for the timing logs and such
unsigned long endTime = 0.0;





void setup() {
  
  Serial.begin(9600); // Initialize Serial communication

  ResetSerialDataBuffer();   // Set Buffer To Null Initially
  SetAllPinsToInput();       // default to all pins as input

  pinMode(ClkPin, OUTPUT);
  setClk(2);
 
  //Serial.println("Welcome to Arduino Data Logger, 1 byte for logg, 7 bytes to control inputs");
  //Serial.println();

  resetStateArray();
  InitializeLastPinStateArray();
}

void setClk(float freq) {  
  TCCR1A = 0x41;                   // Change to use COM1A1:0 bits for OCR1A (pin 9)  
  TCCR1B = 0x14;                   // Same as before, as we are still using Timer 1  
  OCR1A = 0x7A12 / (freq * 2);           // Set frequency for pin 9  
  OCR1B = OCR1A * 0.5;  // This line now affects pin 10 instead of pin 9
  }

void resetStateArray() {
  for (int i = 0; i < numPins/2; i++) {
    currentPinStateArray[i]=0;
    currentPinStateArray[i+7]=0;
  }
}

void SetAllPinsToInput() {
  for (int i = 0; i < numPins/2; i++) {     // Set All Inputs & Outputs pins to INPUT or READ mode
    pinMode(switchPinArray[i] , INPUT);
    pinMode(outputPinArray[i], INPUT);
  }
  
  pinMode(SwitchPowerPin , OUTPUT);         // Turn on Power to switches 
  digitalWrite(SwitchPowerPin , HIGH);
}


void ResetSerialDataBuffer() {
  
  memset(serialDataBuffer, '\0', sizeof(serialDataBuffer));   // Set Buffer To Null
}


void RequestStateLog() {


  //Read and store each state of pin into statArray(is Printed later)
  for (int i = 0; i < numPins/2; i++) {
    currentPinStateArray[i]=digitalRead(switchPinArray[i]);
    currentPinStateArray[i+7]=digitalRead(outputPinArray[i]);
  }

  // Print the pin states to the Serial Monitor
  //Serial.println("    Inputs    |   Outputs   ");
  for (int i = 0; i < numPins/2; i++) {
    Serial.print(currentPinStateArray[i]);
    //Serial.print(" ");
  }
  Serial.print("|");
  for (int i = 0; i < numPins/2; i++) {
    Serial.print(currentPinStateArray[i+7]);
    //Serial.print(" ");
  }
  //endTime = micros();
  Serial.println();
  //Serial.print("Finished With Operation took: ");
  //Serial.print( abs(endTime - startTime));
  //Serial.println("μs");
  //Serial.println();
  
  // Reset Buffer To Null, logging is called at the end of any operation

  ResetSerialDataBuffer(); 
  resetStateArray();
}


void SetSwitchPinsToInput() {              // Sets switch pins to INPUT
  
  for (int i=0; i < numPins/2; i++) {
    digitalWrite(switchPinArray[i],LOW);
    pinMode(switchPinArray[i], INPUT);
  }
  //delay(500);
  digitalWrite(SwitchPowerPin, HIGH);      // turns back on manual mode
}


void SetSwitchPinsToOutput() {             // Set switch pins to OUTPUT
  
  digitalWrite(SwitchPowerPin , LOW);      // turns off manual mode
  //delay(500);
  for (int i=0; i < numPins/2; i++) {
    pinMode(switchPinArray[i], OUTPUT);
  }
}


void ControlMode(const char data[]) {
  unsigned long entryTime = millis(); // Record the time when ControlMode was entered

  do {
    SetSwitchPinsToOutput();

    for (int i = 0; i < 7; ++i) {            // Sets Switches to 1 or 0
      if (data[i] == '1') {
        digitalWrite(switchPinArray[i],HIGH);
      } else if (data[i] == '0') {
        digitalWrite(switchPinArray[i],LOW);
      }
    }

    RequestStateLog(); // Log the current state

    // Minimal delay or perform other necessary operations here
    // Consider reducing or removing this delay to improve responsiveness
    delay(500);

    if (Serial.available() > 0) {
      // If new data is available, read it and reset the entryTime
      ReadSerialData(); // Make sure this function properly populates `serialDataBuffer`
      entryTime = millis(); // Reset entry time since we have a new command
    } else {
      // If no new data, check if the timeout has elapsed
      if (millis() - entryTime >= commandTimeout) {
        break; // Exit the loop if the timeout has elapsed
      }
    }
  } while (true);

  SetSwitchPinsToInput(); // Make sure to set back to input mode when exiting control mode
}


void ReadSerialData() {

  // Read incoming serial data and store it to serialDataBuffer
  char incomingChar = Serial.readBytes(serialDataBuffer, sizeof(serialDataBuffer));
}

void ProcessSerialData() {

  if ((serialDataBuffer[0] == '1' || serialDataBuffer[0] == '0') && (serialDataBuffer[1] != '1' && serialDataBuffer[1] != '0'))  {
      //startTime = micros();
      RequestStateLog();
    }

    //If data is present at 7th value IE last value then controll mode initiallizes with the said value IE inputted data
    if (serialDataBuffer[6] == '1' || serialDataBuffer[6] == '0') {
      //startTime = micros();
      ControlMode(serialDataBuffer);
      ResetSerialDataBuffer();
    }
}

void InitializeLastPinStateArray() {
  for (int i = 0; i < numPins/2; i++) {
    lastPinStateArray[i] = digitalRead(switchPinArray[i]);
  }
}

void CheckForInputChanges() {
  bool stateChanged = false;
  for (int i = 0; i < numPins/2; i++) {
    int currentState = digitalRead(switchPinArray[i]);
    if (currentState != lastPinStateArray[i]) {
      stateChanged = true;
      lastPinStateArray[i] = currentState;
    }
  }
  if (stateChanged) {
    RequestStateLog();
  }
}

void loop() {
  ReadSerialData();
  ProcessSerialData();
  CheckForInputChanges(); // Check for input changes and send state log if changed
}
