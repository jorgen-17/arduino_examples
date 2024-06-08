#include <Arduino.h>
#include "LowPower.h"

int delayTime = 100;
unsigned long trialTime = 60000;
int ledPin = 12;
bool wakeUp = true;
bool goToSleep = false;
bool isAwake = false;
unsigned long previousMillis = 0;

void blink() {
  digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(delayTime);                      // wait for a second
  digitalWrite(ledPin, LOW);   // turn the LED off by making the voltage LOW
  delay(delayTime);                      // wait for a second
  digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(delayTime);  
  digitalWrite(ledPin, LOW);   // turn the LED off by making the voltage LOW
}

void setup() {
  Serial.print("trialTime: ");
  Serial.println(trialTime);
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  Serial.print("currentMillis: ");
  Serial.println(currentMillis);
  Serial.print("previousMillis: ");
  Serial.println(previousMillis);
  Serial.print("currentMillis - previousMillis: ");
  Serial.println(currentMillis - previousMillis);
  Serial.print("currentMillis - previousMillis >= trialtime: ");
  Serial.println((currentMillis - previousMillis) >= trialTime);
  if ((currentMillis - previousMillis) >= trialTime) {
    Serial.println("current trial over, now switching to next trial");
    previousMillis = currentMillis;
    if (isAwake) {
      goToSleep = true;
    } else {
      wakeUp = true;
    }
  }
  if (wakeUp) {
    Serial.println("waking up");
    isAwake = true;
    wakeUp = false;
    blink();
    digitalWrite(ledPin, HIGH);
  } else if (goToSleep) {
    Serial.println("going to sleep now");
    isAwake = false;
    goToSleep = false;
    blink();
    digitalWrite(ledPin, LOW);
    delay(delayTime);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  }


  delay(delayTime);

}
