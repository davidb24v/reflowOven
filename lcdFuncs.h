void lcdSetup() {
  // set the LCD type
  lcd.setMCPType(LTI_TYPE_MCP23008);

  // Set I2C Bus Bit Rate to 400kHz
  TWBR = 12;

  // set up the LCD's number of rows and columns:
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.print(F("Reflow Oven  0.1"));
  lcd.setCursor(0,1);
  lcd.print(F("         by Dave"));
  delay(3000);
  lcd.clear();
 }

void lcdPrintTemperature(double T) {
  int tt = int(T+0.5);
  lcd.setCursor(11,0);
  if ( tt < 100 ) lcd.print(" ");
  if ( tt < 10 ) lcd.print(" ");
  lcd.print(tt);
  lcd.print((char) 223);
  lcd.print("C");
}

void lcdDebugButtons() {
  lcd.setCursor(8,0);
  if ( button1Pressed ) {
    lcd.print((char) 255);
    button1Pressed=false;
  } else {
    lcd.print("_");
  }
  if ( button2Pressed ) {
    lcd.print((char) 255);
    button2Pressed=false;
  } else {
    lcd.print("_");
  }
  if ( button3Pressed ) {
    lcd.print((char) 255);
    button3Pressed=false;
  } else {
    lcd.print("_");
  }
}

