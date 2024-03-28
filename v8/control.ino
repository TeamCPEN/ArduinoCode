void ControlMode(const char data[]) {
  unsigned long entryTime = millis();  // Record the time when ControlMode was entered

  do {
    SetSwitchPinsToOutput();

    for (int i = 0; i < 7; ++i) {  // Sets Switches to 1 or 0
      if (data[i] == '1') {
        digitalWrite(switchPinArray[i], HIGH);
      } else if (data[i] == '0') {
        digitalWrite(switchPinArray[i], LOW);
      }
    }

    RequestStateLog();  // Log the current state

    // Minimal delay or perform other necessary operations here
    // Consider reducing or removing this delay to improve responsiveness
    delay(600);

    if (Serial.available() > 0) {
      // If new data is available, read it and reset the entryTime
      ReadSerialData();  // Make sure this function properly populates `serialDataBuffer`
      if (isValidData(serialDataBuffer)) {
        entryTime = millis();  // Reset entry time since we have a new command
      }
    } else {
      // If no new data, check if the timeout has elapsed
      if (millis() - entryTime >= commandTimeout) {
        break;  // Exit the loop if the timeout has elapsed
      }
    }
  } while (true);

  SetSwitchPinsToInput();  // Make sure to set back to input mode when exiting control mode
}
