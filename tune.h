void displayTuneValue() {
  lcd.setCursor(7,0);
  lcd.print(int(Setpoint));
}

void AutoTuneHelper(boolean start) {
  if(start)
    ATuneModeRemember = myPID.GetMode();
  else
    myPID.SetMode(ATuneModeRemember);
}

void changeAutoTune() {
  if(!tuning)   {
    //Set the output to the desired starting frequency.
    Output = aTuneStartValue;
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    AutoTuneHelper(true);
    tuning = true;
  } else {
    //cancel autotune
    aTune.Cancel();
    tuning = false;
    AutoTuneHelper(false);
  }
}

void printTuningParameters() {
  Serial.print(F("kp="));
  Serial.print(kp);
  Serial.print(" ");
  Serial.print(F("ki="));
  Serial.print(ki);
  Serial.print(" ");
  Serial.print(F("kd="));
  Serial.println(kd);
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
      if ( button3Pressed ) {
        clearAllButtons();
        state = TUNE_RUN_STATE;
        digitalWrite(yellowLed,HIGH);
        displayState(state);
        tuning = false;
        changeAutoTune();
        tuning = true;
        return;
      }
      break;

    case TUNE_SET_STATE:
      // Check for buttons first
      if ( button1Pressed ) {
        clearAllButtons();
        Setpoint -= 10.0;
        Setpoint = max(Setpoint,100.0);
        displayTuneValue();
        return;
      }
      if ( button2Pressed ) {
        clearAllButtons();
        Setpoint += 10.0;
        Setpoint = min(Setpoint,250.0);
        displayTuneValue();
        return;
      }
      if ( button3Pressed ) {
        clearAllButtons();
        state = TUNE_STATE;
        displayState(state);
        return;
      }
      break;

      case TUNE_RUN_STATE:
        // Only button is 3 == QUIT
        if ( button3Pressed ) {
          digitalWrite(RelayPin,LOW);
          digitalWrite(redLed,LOW);
          digitalWrite(yellowLed,LOW);
          clearAllButtons();
          tuning = false;
          AutoTuneHelper(false);
          state = TUNE_STATE;
          displayState(state);
          return;
        }
        if ( tuning ) {
          byte val = aTune.Runtime();
          if ( val != 0 ) {
            tuning = false;
            digitalWrite(yellowLed,LOW);
            Serial.println(F("Tuning complete, old values:"));
            printTuningParameters();
            kp = aTune.GetKp();
            ki = aTune.GetKi();
            kd = aTune.GetKd();
            Serial.println(F("New values:"));
            printTuningParameters();
            myPID.SetTunings(kp, ki, kd);
            AutoTuneHelper(false);
            state = IDLE_STATE;
            displayState(state);
            return;
          } else {
            processOutput();
            break;
          }
        }

    default:
      lcd.setCursor(0,0);
      lcd.print(F("processTuneState"));
  }
}
