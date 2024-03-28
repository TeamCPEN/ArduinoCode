

const int numPins = 14;             // Number of digital pins inputs x 2
int currentPinStateArray[numPins];  // array for state values of pins

int lastPinStateArray[numPins / 2];
char serialDataBuffer[8];  // used as a buffer array

const unsigned long commandTimeout = 5000;  // Timeout period in milliseconds
unsigned long lastCommandTime = 0;

const int switchPinArray[numPins / 2] = { 2, 3, 4, 5, 6, 7, 8 };         // New Pin Numbers
const int outputPinArray[numPins / 2] = { A0, A1, A2, A3, A4, A5, A6 };  // New Pin Numbers
const int SwitchPowerPin = 10;                                           // new pin number
const int ClkPin = 9;

unsigned long startTime = 0.0;  // used for the timing logs and such
unsigned long endTime = 0.0;

float newClockSpeed = 0.0;
bool isClockOn = false;

void setup() {

  Serial.begin(9600);  // Initialize Serial communication

  ResetSerialDataBuffer();  // Set Buffer To Null Initially
  SetAllPinsToInput();      // default to all pins as input

  pinMode(ClkPin, OUTPUT);
  setClk(2);

  resetStateArray();
  InitializeLastPinStateArray();
}

void loop() {
  delay(100);
  ReadSerialData();
  ProcessSerialData();
  if (isClockOn)
    CheckForInputChanges();  // Check for input changes and send state log if changed
}