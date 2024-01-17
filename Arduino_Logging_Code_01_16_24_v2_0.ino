const int numPins = 14;  // Number of digital pins
int stateArray[numPins]; 

const int inputPinArray[numPins/2] = {5,6,7,8,9,10,11};
const int outputPinArray[numPins/2] = {12,13,14,15,16,19,20};

const int SwitchPowerPin = 21;

char dataArray[8];

void setup() {
  Serial.begin(9600);                           // Initialize Serial communication
  memset(dataArray, '\0', sizeof(dataArray));   // Set Buffer To Null Initially

  //Set All Inputs/Outputs to Input inorder to read state value
  for (int i = 0; i < numPins/2; i++) {
    pinMode(inputPinArray[i] , INPUT);
    pinMode(outputPinArray[i], INPUT);
  }

  //Defaults board to User Conntrolled Mode by turning on Switch Power Pin
  pinMode(SwitchPowerPin , OUTPUT);
  digitalWrite(SwitchPowerPin , HIGH);
 
  Serial.println("Welcome to Arduino Data Logger, 1 byte for logg, 7 bytes to control inputs");
}

void loggingMode() {
  //Read and store each state of pin into statArray(is Printed later)
  for (int i = 0; i < numPins/2; i++) {
    stateArray[i]=digitalRead(inputPinArray[i]);
    stateArray[i+7]=digitalRead(outputPinArray[i]);
  }

  // Print the pin states to the Serial Monitor
  //Serial.println("Logging Mode - Pin States Below:");
  for (int i = 0; i < numPins; i++) {
    Serial.print(stateArray[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Reset Buffer To Null, logging is called at the end of any operation
  memset(dataArray, '\0', sizeof(dataArray)); 
}


void controlMode(const char data[]) {
  //I want to make it controlMode(const char data[], boolean x)
  //and if x is true it means that their is more lines to the truth table so it continues instead of closing 
  //and starting again, just a thought tho
  //Disables User Controll Mode and waits for propagation
  digitalWrite(SwitchPowerPin , LOW);
  delay(200);

  //Sets Input Pins to output inorder to simulate truth table
  for (int i=0; i < numPins/2; i++) {
    pinMode(inputPinArray[i], OUTPUT);
  }

  //Depending on the data given to function it sets the corresponding pins to their corresponding values
  for (int i = 0; i < 7; ++i) {
    if (data[i] == '1') {
      digitalWrite(inputPinArray[i],HIGH);
    } else if (data[i] == '0') {
      digitalWrite(inputPinArray[i],LOW);
    }
  }

  //Logging Call so that their is an output of the function
  //Desktop aplication will parse this info currently
  //14 bit string first 7 inputs last 7 outputs
  Serial.println("Result's from inputed TT");
  loggingMode();

  //When this initializes it puts pins back to 0
  //Used to default back to UCM User Controll Mode
  for (int i=0; i < numPins/2; i++) {
    pinMode(inputPinArray[i], INPUT);
  }
  Serial.println("Switching Out of Control Mode");
  delay(1000);
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
  Serial.println("Back to User Mode");
  //I want to add an if(there is a longer truth table) then dont do this 
  //so that its faster becasue of the delay
  digitalWrite(SwitchPowerPin, HIGH);
  } 

void loop() {
    //Read Incoming Serial Data and Store to DataBufferArray
    char incomingChar = Serial.readBytes(dataArray, 8);

    //if data[0] == 1||0 && data[1] != 1||0 do logging mode
    if ((dataArray[0] == '1' || dataArray[0] == '0') && (dataArray[1] != '1' && dataArray[1] != '0'))  {
      Serial.println("Requested Log");
      loggingMode();
    }

    //If data is present at 7th value IE last value then controll mode initiallizes with the said value IE inputted data
    if (dataArray[6] == '1' || dataArray[6] == '0') {
      controlMode(dataArray);
    }

}

