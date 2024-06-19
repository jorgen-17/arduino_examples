#include <Arduino.h>

int delayTime = 100;
int buzzTime = 100;

int buzzButton = 4;
int buzzer = A3;

void buzz () {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 30000; j++) {
      digitalWrite(buzzer, HIGH);
      delayMicroseconds(i);
      digitalWrite(buzzer, LOW);
    }
    delay((buzzTime));
  }
}

void setup () {
  Serial.begin(9600);

  pinMode(buzzButton, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
}

void loop () {
  byte buzzButtonState = digitalRead(buzzButton);
  if (!buzzButtonState) {
    Serial.println("buzz buzz");
    buzz();
  } 

  delay(delayTime);
}
