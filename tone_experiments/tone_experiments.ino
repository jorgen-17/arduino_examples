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
  uint16_t (*changeFunc) (uint16_t note, uint8_t changeAmount, uint16_t initialNote),
  uint8_t changeAmount, uint16_t initialNote, uint16_t finalNote) {
  uint16_t note = initialNote;
  while (note < finalNote) {
    tone(buzzer, note, noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
    note += changeFunc(note, changeAmount, initialNote);
  }
}

void buzzDown (
  uint16_t (*changeFunc) (uint16_t note, uint8_t changeAmount, uint16_t initialNote),
  uint8_t changeAmount, uint16_t initialNote, uint16_t finalNote) {
  uint16_t note = initialNote;
  while (note > finalNote) {
    tone(buzzer, note, noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
    note -= changeFunc(note, changeAmount, initialNote);
  }
}

uint16_t halfStepChange (uint16_t note, uint8_t changeAmount, uint16_t initialNote) {
  return halfStep;
}

uint16_t wholeStepChange (uint16_t note, uint8_t changeAmount, uint16_t initialNote) {
  return wholeStep;
}

uint16_t upWholeDownHalf (uint16_t note, uint8_t changeAmount, uint16_t initialNote) {
  if (stepUp) {
    stepUp = false;
    return wholeStep * 3;
  }
  stepUp = true;
  return halfStep * 3;
}

// fibonacci series
uint16_t buzzonacci (uint16_t note, uint8_t changeAmount, uint16_t initialNote) {
  uint16_t current;
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
uint16_t buzzalan (uint16_t note, uint8_t changeAmount, uint16_t initialNote) {
  if (note == initialNote) {
    previous = 1;
  }
  previous += (previous + 2) / 2;
  return previous * changeAmount;
}

uint16_t squareBuzz (uint16_t note, uint8_t changeAmount, uint16_t initialNote) {
  if (note == initialNote) {
    previous = 1;
  }
  previous += (previous * (previous * previous + 1)) / 2;
  return previous * changeAmount;
}

uint16_t triangleBuzz (uint16_t note, uint8_t changeAmount, uint16_t initialNote) {
  if (note == initialNote) {
    previous = 1;
  }
  previous += (previous * (previous + 1)) / 2;
  return previous * changeAmount;
}

void franticRobot () {
  uint16_t start = 223;
  uint16_t end = 762;
  uint8_t sequence[11] = { 1, 2, 3, 5, 8, 13, 8, 5, 3, 2, 1 };
  uint8_t sequenceLength = sizeof(sequence) / sizeof(sequence[0]);
  for (uint8_t i = 0; i < sequenceLength; i++) {
    uint16_t pitchChange = halfStep * sequence[i];
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

void ascendingTrill () {
  uint16_t start = 223;
  uint16_t end = 762;
  uint8_t prev = 1;
  uint8_t curr = 1;
  while (curr <= 21) {
    uint16_t pitchChange = halfStep * curr;
    start += pitchChange;
    end += pitchChange;
    buzzUp(wholeStepChange, seventhStep, start, end);
    delay(delayTime);
    prev = curr;
    curr += prev;
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
    ascendingTrill();
  }

  delay(delayTime);
}
