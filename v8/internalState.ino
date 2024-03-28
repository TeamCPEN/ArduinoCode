void RequestStateLog() {

  // Read and store each state of pin into statArray(is Printed later)
  for (int i = 0; i < numPins / 2; i++) {
    currentPinStateArray[i] = digitalRead(switchPinArray[i]);
    if (i != 6) {
      currentPinStateArray[i + 7] = digitalRead(outputPinArray[i]);
    } else {
      const int k = analogRead(outputPinArray[i]);
      int m = 0;
      if (k > 300) {
        m = 1;
      }
      currentPinStateArray[i + 7] = m;
    }
  }

  // Print the pin states to the Serial Monitor
  // Serial.println("    Inputs    |   Outputs   ");
  for (int i = 0; i < numPins / 2; i++) {
    Serial.print(currentPinStateArray[i]);
    // Serial.print(" ");
  }
  Serial.print("|");
  for (int i = 0; i < numPins / 2; i++) {
    Serial.print(currentPinStateArray[i + 7]);
    // Serial.print(" ");
  }
  // endTime = micros();
  Serial.println();
  // Serial.print("Finished With Operation took: ");
  // Serial.print( abs(endTime - startTime));
  // Serial.println("μs");
  // Serial.println();

  // Reset Buffer To Null, logging is called at the end of any operation

  ResetSerialDataBuffer();
  resetStateArray();
}

void SetSwitchPinsToInput() {  // Sets switch pins to INPUT

  for (int i = 0; i < numPins / 2; i++) {
    digitalWrite(switchPinArray[i], LOW);
    pinMode(switchPinArray[i], INPUT);
  }
  // delay(500);
  digitalWrite(SwitchPowerPin, HIGH);  // turns back on manual mode
}

void SetSwitchPinsToOutput() {  // Set switch pins to OUTPUT

  digitalWrite(SwitchPowerPin, LOW);  // turns off manual mode
  // delay(500);
  for (int i = 0; i < numPins / 2; i++) {
    pinMode(switchPinArray[i], OUTPUT);
  }
}

void resetStateArray() {
  for (int i = 0; i < numPins / 2; i++) {
    currentPinStateArray[i] = 0;
    currentPinStateArray[i + 7] = 0;
  }
}

void SetAllPinsToInput() {
  for (int i = 0; i < numPins / 2; i++) {  // Set All Inputs & Outputs pins to INPUT or READ mode
    pinMode(switchPinArray[i], INPUT);
    pinMode(outputPinArray[i], INPUT);
  }

  pinMode(SwitchPowerPin, OUTPUT);  // Turn on Power to switches
  digitalWrite(SwitchPowerPin, HIGH);
}

void InitializeLastPinStateArray() {
  for (int i = 0; i < numPins / 2; i++) {
    lastPinStateArray[i] = digitalRead(switchPinArray[i]);
  }
}

void CheckForInputChanges() {
  bool stateChanged = false;
  for (int i = 0; i < numPins / 2; i++) {
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