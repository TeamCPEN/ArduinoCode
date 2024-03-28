
void ResetSerialDataBuffer() {

  memset(serialDataBuffer, '\0', sizeof(serialDataBuffer));  // Set Buffer To Null
}



bool isValidData(const char *data) {
  // Example validation: check data length and content
  for (int i = 0; i < 7; i++) {
    if (data[i] != '0' && data[i] != '1') {
      return false;  // Invalid data
    }
  }
  return true;  // Valid data
}


void ReadSerialData() {
  // Clear the buffer first
  memset(serialDataBuffer, '\0', sizeof(serialDataBuffer));

  // Then read new data into the buffer
  if (Serial.available() > 0) {
    Serial.readBytes(serialDataBuffer, sizeof(serialDataBuffer) - 1);
  }
}

void ProcessSerialData() {



  if ((serialDataBuffer[0] == '1' || serialDataBuffer[0] == '0') && (serialDataBuffer[1] != '1' && serialDataBuffer[1] != '0')) {
    // startTime = micros();
    RequestStateLog();
  }

  // If data is present at 7th value IE last value then controll mode initiallizes with the said value IE inputted data
  if (serialDataBuffer[6] == '1' || serialDataBuffer[6] == '0') {
    // startTime = micros();
    ControlMode(serialDataBuffer);
    ResetSerialDataBuffer();
  }

  if (serialDataBuffer[0] == '#' && serialDataBuffer[6] == '\0')
    offClk();

  if (serialDataBuffer[0] == '#' && serialDataBuffer[1] == '#')
    setClk(2);

  if (serialDataBuffer[0] == '?')
    Serial.println("1V8");

  if (serialDataBuffer[0] == 'x') {
    cycleAndState();
  }

  if (serialDataBuffer[0] == 'f') {
    String stringOne = String(serialDataBuffer);
    int len = stringOne.length();
    if (len == 3 || len == 4) {
      float newFreq = stringOne.substring(1, len - 1).toFloat();
      if (newFreq < 0 || newFreq > 30) {
      } else {
        setClk(newFreq);
      }
    }
  }
}
