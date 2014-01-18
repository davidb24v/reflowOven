/*

This file defines the menus corresponding to each state. It's nothing fancy, just here to
keep the main file free of crap...

*/

void displayState(int state) {
  switch (state) {
    case IDLE_STATE:
      // IDLE state
      lcd.setCursor(0,0);
      lcd.print(F("IDLE      "));
      // IDLE Menus
      lcd.setCursor(0,1);
      lcd.print(F("Tune  Show   Run"));
      break;

    case TUNE_STATE:
      // TUNE state
      lcd.setCursor(0,0);
      lcd.print(F("TUNE      "));
      // TUNE Menus
      lcd.setCursor(0,1);
      lcd.print(F("Back  Set     Go"));
      break;

    case TUNE_SET_STATE:
      // TUNE state
      lcd.setCursor(0,0);
      lcd.print(F("Setpt:    "));
      lcd.setCursor(7,0);
      lcd.print(int(Setpoint));
      // TUNE Target Menus
      lcd.setCursor(0,1);
      lcd.print(F(" -    +     Done"));
      break;

    case RUN_STATE:
      // Basic RUN state
      lcd.setCursor(0,0);
      lcd.print(F("PRE-HEAT  "));
      // IDLE Menus
      lcd.setCursor(0,1);
      lcd.print(F("            STOP"));
      break;

    default:
      // Not written yet...
      lcd.setCursor(0,0);
      lcd.print(F("Unknown!  "));
      // IDLE Menus
      lcd.setCursor(0,1);
      lcd.print(F("Arse  Arse  Arse"));
  }
}
