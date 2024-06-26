#include <Arduino.h>
#include "LowPower.h"

int delayTime = 100;
bool isAwake = true;
int brightness = 5;
int buzzTime = 100;
int buzzTime2 = 200;

int greenLed = 5;
int awakeButton = 2;
int sleepButton = 3;
int buzzer = 13;

void wakeUp () {
  isAwake = true;
}

void goToSleep () {
  isAwake = false;
}

void buzz () {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j )
  }
}

void setup () {
  Serial.begin(9600);

  pinMode(greenLed, OUTPUT);
  pinMode(sleepButton, INPUT_PULLUP);
  pinMode(awakeButton, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  analogWrite(greenLed, brightness);
  attachInterrupt(digitalPinToInterrupt(awakeButton), wakeUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(sleepButton), goToSleep, FALLING);
}

void loop () {
  if (isAwake) {
    Serial.println("awake");
    analogWrite(greenLed, brightness);
  } else {
    Serial.println("asleep");
    analogWrite(greenLed, 0);

    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  }

  delay(delayTime);
}
