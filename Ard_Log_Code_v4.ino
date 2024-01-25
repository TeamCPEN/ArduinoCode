const int numPins = 14;                   // Number of digital pins inputs x 2 
int currentPinStateArray[numPins];        // array for state values of pins
char serialDataBuffer[8];                 //used as a buffer array

const int switchPinArray[numPins/2] = {2,3,4,5,6,7,8};         //New Pin Numbers
const int outputPinArray[numPins/2] = {9,10,11,12,13,14,15};  //New Pin Numbers
const int SwitchPowerPin = 16;                                //new pin number

unsigned long startTime = 0.0;  //used for the timing logs and such
unsigned long endTime = 0.0;


void setup() {
  
  Serial.begin(9600); // Initialize Serial communication

  ResetSerialDataBuffer();   // Set Buffer To Null Initially
  SetAllPinsToInput();       // default to all pins as input
 
  //Serial.println("Welcome to Arduino Data Logger, 1 byte for logg, 7 bytes to control inputs");
  //Serial.println();
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
  endTime = micros();
  Serial.println();
  //Serial.print("Finished With Operation took: ");
  //Serial.print( abs(endTime - startTime));
  //Serial.println("μs");
  //Serial.println();
  
  // Reset Buffer To Null, logging is called at the end of any operation

  ResetSerialDataBuffer(); 
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

  SetSwitchPinsToOutput();

  for (int i = 0; i < 7; ++i) {            // Sets Switches to 1 or 0
    if (data[i] == '1') {
      digitalWrite(switchPinArray[i],HIGH);
    } else if (data[i] == '0') {
      digitalWrite(switchPinArray[i],LOW);
    }
  }

  RequestStateLog();                       // Sends log to Serial

  //delay(500);                             // So we can physically see what is happening 


  // This is where we would need to route back to the loop
  // if there were more TT rows following works without but will increase performance
  
  SetSwitchPinsToInput();
  } 


void ReadSerialData() {

  // Read incoming serial data and store it to serialDataBuffer
  char incomingChar = Serial.readBytes(serialDataBuffer, sizeof(serialDataBuffer));
}

void ProcessSerialData() {

  if ((serialDataBuffer[0] == '1' || serialDataBuffer[0] == '0') && (serialDataBuffer[1] != '1' && serialDataBuffer[1] != '0'))  {
      startTime = micros();
      RequestStateLog();
    }

    //If data is present at 7th value IE last value then controll mode initiallizes with the said value IE inputted data
    if (serialDataBuffer[6] == '1' || serialDataBuffer[6] == '0') {
      startTime = micros();
      ControlMode(serialDataBuffer);
    }
}

void loop() {
 
  ReadSerialData();

  ProcessSerialData();

}
