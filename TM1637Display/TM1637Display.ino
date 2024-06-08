#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define DIO 4
#define CLK 5
#define relay 13


// The amount of time (in milliseconds) between tests
#define TEST_DELAY   3000

const uint8_t SEG_DONE[] = {
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_C | SEG_E | SEG_G,                           // n
	SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
	};
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
  SEG_C | SEG_D | SEG_E,                           // u
};
const uint8_t COLON_DOTS = 0b01000000; // :

TM1637Display display(CLK, DIO);

void setup()
{
  Serial.begin(9600);

  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
}

void loop()
{
  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

  // All segments on
  Serial.println("All segments on");
  display.setSegments(data);
  delay(TEST_DELAY);

  // Selectively set different digits
  display.setBrightness(0);

  Serial.println("Displaying time: 12:34");
  display.showNumberDecEx(1234, COLON_DOTS, true);
  delay(TEST_DELAY);

  Serial.println("Displaying time: 1:23");
  display.showNumberDecEx(123, COLON_DOTS, true);
  delay(TEST_DELAY);

  
  for(k = 3; k >= 0; k--) {
	display.setSegments(data, 1, k);
	delay(TEST_DELAY);
	}
  

  display.clear();
  display.setSegments(data+2, 2, 2);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data+2, 2, 1);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data+1, 3, 1);
  delay(TEST_DELAY);


  // Show decimal numbers with/without leading zeros
  display.showNumberDec(0, false); // Expect: ___0
  delay(TEST_DELAY);
  display.showNumberDec(0, true);  // Expect: 0000
  delay(TEST_DELAY);
	display.showNumberDec(1, false); // Expect: ___1
	delay(TEST_DELAY);
  display.showNumberDec(1, true);  // Expect: 0001
  delay(TEST_DELAY);
  display.showNumberDec(301, false); // Expect: _301
  delay(TEST_DELAY);
  display.showNumberDec(301, true); // Expect: 0301
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(14, false, 2, 1); // Expect: _14_
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(4, true, 2, 2);  // Expect: __04
  delay(TEST_DELAY);
  display.showNumberDec(-1, false);  // Expect: __-1
  delay(TEST_DELAY);
  display.showNumberDec(-12);        // Expect: _-12
  delay(TEST_DELAY);
  display.showNumberDec(-999);       // Expect: -999
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(-5, false, 3, 0); // Expect: _-5_
  delay(TEST_DELAY);
  display.showNumberHexEx(0xf1af);        // Expect: f1Af
  delay(TEST_DELAY);
  display.showNumberHexEx(0x2c);          // Expect: __2C
  delay(TEST_DELAY);
  display.showNumberHexEx(0xd1, 0, true); // Expect: 00d1
  delay(TEST_DELAY);
  display.clear();
  display.showNumberHexEx(0xd1, 0, true, 2); // Expect: d1__
  delay(TEST_DELAY);
  
	// Run through all the dots
	for(k=0; k <= 4; k++) {
    Serial.print("Run through all the dots: ");
    Serial.println(k);
		display.showNumberDecEx(0, (0x80 >> k), true);
		delay(TEST_DELAY);
	}

  // Brightness Test
  for(k = 0; k < 4; k++)
	data[k] = 0xff;
  for(k = 0; k < 7; k++) {
    Serial.print("Brightness Test: ");
    Serial.println(k);
    display.setBrightness(k);
    display.setSegments(data);
    delay(TEST_DELAY);
  }
  
  // On/Off test
  Serial.println("On/Off test");
  for(k = 0; k < 4; k++) {
    Serial.println("   Off");
    display.setBrightness(7, false);  // Turn off
    display.setSegments(data);
    delay(TEST_DELAY);
    Serial.println("   ON");
    display.setBrightness(7, true); // Turn on
    display.setSegments(data);
    delay(TEST_DELAY);  
  }
 
  // Done!
  display.setSegments(SEG_DONE);
  delay(TEST_DELAY);
  display.setSegments(SEG_GOOD);
  delay(TEST_DELAY);
  display.setSegments(SEG_BYE);
  delay(TEST_DELAY);
  display.setSegments(SEG_NOW);
  delay(TEST_DELAY);

  while(1);
}