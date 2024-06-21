#include <Arduino.h>

uint8_t delayTime = 100;
uint8_t noteDuration = 77;
uint8_t buzzTime = 100;
uint8_t previous = 1;
uint8_t seventhStep = 9;
uint8_t halfStep = 31;
uint8_t wholeStep = 62;
bool stepUp = true;

uint8_t buzzButton = 4;
uint8_t buzzer = 11;

void buzzUp (
  int (*changeFunc) (int note, uint8_t changeAmount, int initialNote),
  uint8_t changeAmount, int initialNote, int finalNote) {
  int note = initialNote;
  while (note < finalNote) {
    tone(buzzer, note, noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
    note += changeFunc(note, changeAmount, initialNote);
  }
}

void buzzDown (
  int (*changeFunc) (int note, uint8_t changeAmount, int initialNote),
  uint8_t changeAmount, int initialNote, int finalNote) {
  int note = initialNote;
  while (note > finalNote) {
    tone(buzzer, note, noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
    note -= changeFunc(note, changeAmount, initialNote);
  }
}

int halfStepUp (int note, uint8_t changeAmount, int initialNote) {
  return halfStep;
}

int wholeStepUp (int note, uint8_t changeAmount, int initialNote) {
  return wholeStep;
}

int upWholeDownHalf (int note, uint8_t changeAmount, int initialNote) {
  if (stepUp) {
    stepUp = false;
    return wholeStep * 3;
  }
  stepUp = true;
  return halfStep * 3;
}

// fibonacci series
int buzzonacci (int note, uint8_t changeAmount, int initialNote) {
  int current;
  if (note == initialNote) {
    previous = 1;
    current = 1;
  } else {
    current = abs((note - initialNote) / changeAmount);
  }
  previous += current;
  return previous * changeAmount;
}

// catalan numbers series
int buzzalan (int note, uint8_t changeAmount, int initialNote) {
  if (note == initialNote) {
    previous = 1;
  }
  previous += (previous + 2) / 2;
  return previous * changeAmount;
}

int squareBuzz (int note, uint8_t changeAmount, int initialNote) {
  if (note == initialNote) {
    previous = 1;
  }
  previous += (previous * (previous * previous + 1)) / 2;
  return previous * changeAmount;
}

int triangleBuzz (int note, uint8_t changeAmount, int initialNote) {
  if (note == initialNote) {
    previous = 1;
  }
  previous += (previous * (previous + 1)) / 2;
  return previous * changeAmount;
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
    int start = 223;
    int end = 762;
    uint8_t sequence[11] = { 1, 2, 3, 5, 8, 13, 8, 5, 3, 2, 1 };
    uint8_t sequenceLength = sizeof(sequence) / sizeof(sequence[0]);
    for (uint8_t i = 0; i < sequenceLength; i++) {
      int pitchChange = halfStep * sequence[i];
      if (pitchChange % 2) {
        start += pitchChange;
        end += pitchChange;
        buzzDown(buzzonacci, seventhStep, end, start);
      } else {
        start -= pitchChange;
        end -= pitchChange;
        buzzUp(buzzalan, seventhStep, start, end);
      }
    }
  }

  delay(delayTime);
}
