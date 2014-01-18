/********************************************************
 * PID RelayOutput Example
 * Same as basic example, except that this time, the output
 * is going to a digital pin which (we presume) is controlling
 * a relay.  the pid is designed to Output an analog value,
 * but the relay can only be On/Off.
 *
 *   to connect them together we use "time proportioning
 * control"  it's essentially a really slow version of PWM.
 * first we decide on a window size (5000mS say.) we then 
 * set the pid to adjust its output between 0 and that window
 * size.  lastly, we add some logic that translates the PID
 * output into "Relay On Time" with the remainder of the 
 * window being "Relay Off Time"
 ********************************************************/

#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#define RelayPin 16

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 300, 0.05, 250, DIRECT);
PID_ATune aTune(&Input, &Output);

int WindowSize = 5000;
unsigned long windowStartTime;

// include the library code for an LCD display
#include <Wire.h>
#include <LiquidTWI2.h>

// Connect via i2c
LiquidTWI2 lcd(0);

// State variable
/*
      State    Menus                 Description
          0    Tune | Show | Run     Initial IDLE State
        100    Back | Set  | Go      TUNE State
        200     -   |  +   | Done    TUNE_SET state
      10000         |      | QUIT    Tuning State
      20000         |      | STOP    Running State
*/

int state = 0;
#define IDLE_STATE 0
#define TUNE_STATE 100
#define TUNE_SET_STATE  200
#define TUNE_END_STATE 999
#define TUNE_RUN_STATE 10000
#define RUN_STATE 20000
#include "menus.h"

const int Vref = 5;

// LEDs
const int redLed=13;
const int yellowLed=12;
const int greenLed=11;

// Buttons
const int Button1=8;
const int Button2=9;
const int Button3=10;

// Flags to indicate button pressed (actually, it indicates "released", not "pressed" but who cares?)
bool button1Pressed = false;
bool button2Pressed = false;
bool button3Pressed = false;

// Helper code for buttons
#define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
#define NO_PORTD_PINCHANGES // to indicate that port d will not be used for pin change interrupts
#define NO_PIN_STATE
#define NO_PIN_NUMBER
#include <PinChangeInt.h>
#include "buttons.h"

void clearAllButtons() {
  button1Pressed = false;
  button2Pressed = false;
  button3Pressed = false;
}


// When to update display
const int printInterval = 250;
long nextPrint = 0;

// LCD helper functions
#include "lcdFuncs.h"

// Tuning helper functions
#include "tune.h"

void setup()
{
  // Relay pin
  digitalWrite(RelayPin, LOW);
  pinMode(RelayPin, OUTPUT);

  // LEDs
  pinMode(redLed,OUTPUT);
  pinMode(yellowLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
  
  // Turn green LED on
  digitalWrite(greenLed, HIGH);

  windowStartTime = millis();
  
  //initialize the variables we're linked to
  Setpoint = 150;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(MANUAL);

  // Analogue 0 connects to the thermocouple amplifier
  Serial.begin(115200);
  pinMode(A0, INPUT);

  // Initialise LCD
  lcdSetup();

  displayState(state);

  // Setup buttons
  buttonSetup();
}

double degC() {
  int val = analogRead(A0);
  // Apply reference voltage, scale to 10mV (== degC) and
  // divide out the analogue range (1024)
  double temp = (val * Vref * 100.0) / 1024.0;
  return temp;
}

void loop()
{
  Input = degC();
  
  long now = millis();
  if ( now > nextPrint ) {
    lcdPrintTemperature(Input);
    //lcdDebugButtons();
    Serial.print(Input);
    Serial.print(" ");
    Serial.println(Output);
    nextPrint += printInterval;
  }

  if ( state == IDLE_STATE ) {
    // Check for buttons first
    if ( button1Pressed ) {
      button1Pressed = false;
      state = TUNE_STATE;
      displayState(state);
      return;
    }
  }

  if ( state >= TUNE_STATE && state < TUNE_END_STATE ) {
    processTuneState();
  }

  /************************************************
   * turn the output pin on/off based on pid output
   ************************************************/
  if ( state >= RUN_STATE ) {
    myPID.Compute();
    if (millis() - windowStartTime>WindowSize) {
      //time to shift the Relay Window
      windowStartTime += WindowSize;
    }
    if (Output > (millis() - windowStartTime) ) {
      digitalWrite(RelayPin,HIGH);
      digitalWrite(redLed, HIGH);
    } else {
      digitalWrite(RelayPin,LOW);
      digitalWrite(redLed, LOW);
    }
  }
}



