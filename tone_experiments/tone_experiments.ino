#include <Arduino.h>

int delayTime = 100;
int noteDuration = 350;
int buzzTime = 100;
int previous = 1;
int seventhStep = 9;
int halfStep = 31;
int wholeStep = 62;
bool stepUp = true;

int buzzButton = 4;
int buzzer = 11;

void buzzUp (
  int (*incrementFunc) (int note, int incrementAmount, int initialNote),
  int incrementAmount, int initialNote, int finalNote) {
  int note = initialNote;
  while (note < finalNote) {
    tone(buzzer, note, noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
    note = incrementFunc(note, incrementAmount, initialNote);
    Serial.print("note: ");
    Serial.println(note);
  }
}

int halfStepUp (int note, int incrementAmount, int initialNote) {
  return note + halfStep;
}

int wholeStepUp (int note, int incrementAmount, int initialNote) {
  return note + wholeStep;
}

int upWholeDownHalf (int note, int incrementAmount, int initialNote) {
  if (stepUp) {
    stepUp = false;
    return note + wholeStep * 3;
  }
  stepUp = true;
  return note - halfStep * 3;
}

// fibonacci series
int buzzonacci (int note, int incrementAmount, int initialNote) {
  int current;
  if (note == initialNote) {
    previous = 1;
    current = 1;
  } else {
    current = (note - initialNote) / incrementAmount;
  }
  previous += current;
  return note + (previous * incrementAmount);
}

// catalan numbers series
int buzzalan (int note, int incrementAmount, int initialNote) {
  if (note == initialNote) {
    previous = 1;
  }
  previous += (previous + 2) / 2;
  return note + (previous * incrementAmount);
}

int squareBuzz (int note, int incrementAmount, int initialNote) {
  if (note == initialNote) {
    previous = 1;
  }
  previous += (previous * (previous * previous + 1)) / 2;
  return note + (previous * incrementAmount);
}

int triangleBuzz (int note, int incrementAmount, int initialNote) {
  if (note == initialNote) {
    previous = 1;
  }
  previous += (previous * (previous + 1)) / 2;
  return note + (previous * incrementAmount);
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
    for (int i = 0; i < 3; i++) {
      buzzUp(buzzonacci, seventhStep, 523, 3515);
      buzzUp(buzzonacci, seventhStep, 1015, 4515);
      buzzUp(buzzonacci, seventhStep, 1523, 5515);
      delay(delayTime * 3);
    }
  }

  delay(delayTime);
}