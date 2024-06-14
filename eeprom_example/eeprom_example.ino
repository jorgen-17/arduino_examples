#include <EEPROM.h>

struct checkpoint {
  uint32_t easting;
  uint32_t northing;
  uint16_t heading;
};
byte checkpointAddress = 0;
byte clearButtonPin = 4;

void writeToEEPROM () {
  Serial.println("writing to the EEPROM");
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, i);
  }
}

boolean isEEPROMEmpty () {
  uint32_t sum = 0;
  byte value;
  for (int i = 0; i < EEPROM.length(); i++) {
    value = EEPROM.read(i);
    if (value >= 0) {
      return false;
    }
    sum += value;
  }
  return sum == 0;
}

void printEEPROM () {
  Serial.println("reading from EEPROM");
  byte value;
  for (int i = 0; i < EEPROM.length(); i++) {
    value = EEPROM.read(i);
    Serial.println(value, DEC);
  }
}

void clearEEPROM () {
  Serial.println("clearing the EEPROM");
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  Serial.println("EEPROM cleared");
}

void testEEPROMSequential () {
  if (isEEPROMEmpty()) {
    Serial.println("EEPROM is empty");
    printEEPROM();
    writeToEEPROM();
  } else {
    Serial.println("EEPROM is NOT empty");
    printEEPROM();
  }
}

checkpoint getCheckpoint () {
  checkpoint check;
  EEPROM.get(checkpointAddress, check);
  return check;
}

void printCheckpoint (checkpoint check) {
  Serial.print("easting:");
  Serial.println(check.easting);
  Serial.print("northing:");
  Serial.println(check.northing);
  Serial.print("heading:");
  Serial.println(check.heading);  
}

void saveCheckpoint () {
  Serial.println("saving checkpoint");
  checkpoint check = {
    .easting = 35,
    .northing = 55,
    .heading = 315
  };
  EEPROM.put(checkpointAddress, check);
}

void testEEPROMStruct () {
  checkpoint check = getCheckpoint();
  if (check.easting == 0 && check.northing == 0 && check.heading == 0) {
    Serial.println("checkpoint NOT found");
    saveCheckpoint();
  } else {
    Serial.println("checkpoint FOUND");
    printCheckpoint(check);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(clearButtonPin, INPUT_PULLUP);

  // testEEPROMSequential();
  testEEPROMStruct();
}

void loop() {
  byte clearButtonState = digitalRead(clearButtonPin);
  if (!clearButtonState) {
    Serial.println("clear button pressed");
    clearEEPROM();
    printEEPROM();
  }
}
