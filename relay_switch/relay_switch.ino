#include <Arduino.h>
#include "LowPower.h"
#include <Stepper.h>
#include <DS3231.h>
#include <Wire.h>
#include <TM1637Display.h>

int delayTime = 100;
bool isAwake;
bool wakeUp = true;
bool goToSleep = false;
int awakeButton = 2;
int sleepButton = 3;

int displayDIO = 4;
int displayCLK = 5;

/*
    -A-
  F|   |B
    -G-
  E|   |C
    -D-
*/
const uint8_t SEG_GOOD[] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,   // g
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G            // d
};
const uint8_t SEG_BYE[] = {
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,           // b
  SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,           // y
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  0b00000000                                       //
};
const uint8_t SEG_NOW[] = {
  SEG_C | SEG_E | SEG_G,                           // n
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_D | SEG_E,                           // u
  SEG_C | SEG_D | SEG_E                            // u
};
const uint8_t SEG_HOLA[] = {
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,           // H
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_D | SEG_E | SEG_F,                           // L
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G    // A
};
const uint8_t COLON_DOTS = 0b01000000; // :
TM1637Display display(displayCLK, displayDIO);
int displayDelay = 3000;

int in1Pin = 9;
int in2Pin = 10;
int in3Pin = 11;
int in4Pin = 12;
int stepsPerRevolution = 2048;
int stepSpeed = 5;
int stepsPerLoop = 25;
Stepper stepper(stepsPerRevolution, in1Pin, in3Pin, in2Pin, in4Pin);

DS3231 myClock;
RTClib myRTC;
bool century = false;
bool h12Flag = false;
bool pmFlag = false;

byte year = 23;
byte month = 8;
byte date = 17;
byte DOW = 4;
byte hour = 22;
byte minute = 49;
byte second = 0;

int relay = 13;

void wakeUpInterrupt () {
  wakeUp = true;
}

void goToSleepInterrupt () {
  goToSleep = true;
}

float celciusToFarenheit (int c) {
  return (c * 9.0/5) + 32;
}

void printTime () {
  Serial.print("Time: ");

	// then the month
	Serial.print(myClock.getMonth(century), DEC);
	Serial.print("/");
  
	// then the date
	Serial.print(myClock.getDate(), DEC);
	Serial.print("/");

  // then the year
  Serial.print(myClock.getYear(), DEC);
	Serial.print(" ");
  
	// Finally the hour, minute, and second
  int hour = myClock.getHour(h12Flag, pmFlag);
  int minute = myClock.getMinute();
  int second = myClock.getSecond();
	Serial.print(hour, DEC);
	Serial.print(":");
	Serial.print(minute, DEC);
	Serial.print(":");
	Serial.print(second, DEC);
	Serial.print(" ");

  // Display the temperature
	Serial.print("Temp: ");
	Serial.print(celciusToFarenheit(myClock.getTemperature()), 2);
  Serial.println("F");

  int time = hour * 100 + minute;
  if (second >= 60) {
    display.clear();
  }
  display.showNumberDecEx(time, COLON_DOTS, true);
}

void powerDownDisplay() {
  display.setSegments(SEG_GOOD);
  delay(displayDelay);
  display.setSegments(SEG_BYE);
  delay(displayDelay);
  display.setSegments(SEG_NOW);
  delay(displayDelay);

  display.setBrightness(0, false);  // Turn off
  delay(delayTime);
}

void powerUpDisplay() {
  display.setBrightness(0, true);  // Turn on
  delay(delayTime);
  display.setSegments(SEG_HOLA);
  delay(displayDelay);
}

void setup() {
  Serial.begin(9600);

  pinMode(awakeButton, INPUT_PULLUP);
  pinMode(sleepButton, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  delay(delayTime);
  stepper.setSpeed(stepSpeed);

  display.setBrightness(0);

  Wire.begin();

  myClock.setClockMode(h12Flag);
  myClock.setYear(year);
  myClock.setMonth(month);
  myClock.setDate(date);
  myClock.setDoW(DOW);
  myClock.setHour(hour);
  myClock.setMinute(minute);
  myClock.setSecond(second);

  attachInterrupt(digitalPinToInterrupt(awakeButton), wakeUpInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(sleepButton), goToSleepInterrupt, FALLING);
}

void loop() {
  if (wakeUp) {
    wakeUp = false;
    isAwake = true;
    Serial.println("awake");
    digitalWrite(relay, HIGH);
    delay(delayTime);
    powerUpDisplay();
  }
  if (goToSleep) {
    goToSleep = false;
    isAwake = false;
    Serial.println("asleep");
    powerDownDisplay();
    analogWrite(relay, LOW);
    delay(delayTime);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  }
  if (isAwake) {
    digitalWrite(relay, HIGH);
    stepper.step(stepsPerLoop);
    printTime();
  }
  delay(delayTime);
}
