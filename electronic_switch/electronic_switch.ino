#include <Arduino.h>
#include "LowPower.h"
#include <Stepper.h>
#include <DS3231.h>
#include <Wire.h>

int delayTime = 100;
bool isAwake;
bool wakeUp = true;
bool goToSleep = false;
int brightness = 50;

int awakeButton = 2;
int sleepButton = 3;
int mosfet = 4;

int in1Pin = 9;
int in2Pin = 10;
int in3Pin = 11;
int in4Pin = 12;
int stepsPerRevolution = 2048;
int stepSpeed = 5;
int stepsPerLoop = 250;
Stepper stepper(stepsPerRevolution, in1Pin, in3Pin, in2Pin, in4Pin);

DS3231 myClock;
RTClib myRTC;
bool century = false;
bool h12Flag = false;
bool pmFlag = false;

byte year = 22;
byte month = 12;
byte date = 6;
byte DOW = 4;
byte hour = 20;
byte minute = 23;
byte second = 0;

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
	Serial.print(myClock.getHour(h12Flag, pmFlag), DEC);
	Serial.print(":");
	Serial.print(myClock.getMinute(), DEC);
	Serial.print(":");
	Serial.print(myClock.getSecond(), DEC);
	Serial.print(" ");

  // Display the temperature
	Serial.print("Temp: ");
	Serial.print(celciusToFarenheit(myClock.getTemperature()), 2);
  Serial.println("F");
}

void setup() {
  Serial.begin(9600);

  pinMode(awakeButton, INPUT_PULLUP);
  pinMode(sleepButton, INPUT_PULLUP);
  pinMode(mosfet, OUTPUT);
  digitalWrite(mosfet, HIGH);

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
    digitalWrite(mosfet, HIGH);
    delay(delayTime);
  }
  if (goToSleep) {
    goToSleep = false;
    isAwake = false;
    Serial.println("asleep");
    analogWrite(mosfet, LOW);
    delay(delayTime);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  }
  if (isAwake) {
    printTime();
  }
  delay(delayTime);
}
