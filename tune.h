void displayTuneValue() {
  lcd.setCursor(7,0);
  lcd.print(int(Setpoint));
}

void processTuneState() {
  switch (state) {
    case TUNE_STATE:
      // Check for buttons first
      if ( button1Pressed ) {
        clearAllButtons();
        state = IDLE_STATE;
        displayState(state);
        return;
      }
      if ( button2Pressed ) {
        clearAllButtons();
        state = TUNE_SET_STATE;
        displayState(state);
        return;
      }
      break;

    case TUNE_SET_STATE:
      // Check for buttons first
      if ( button1Pressed ) {
        button1Pressed = false;
        Setpoint -= 10.0;
        Setpoint = max(Setpoint,100.0);
        displayTuneValue();
        return;
      }
      if ( button2Pressed ) {
        button2Pressed = false;
        Setpoint += 10.0;
        Setpoint = min(Setpoint,250.0);
        displayTuneValue();
        return;
      }
      if ( button3Pressed ) {
        button3Pressed = false;
        state = TUNE_STATE;
        displayState(state);
        return;
      }
      break;

    default:
      lcd.setCursor(0,0);
      lcd.print(F("processTuneState"));
  }
}
