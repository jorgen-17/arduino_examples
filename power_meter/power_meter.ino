#include <Adafruit_GFX.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>
#include <SdFat.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_INA219 ina219;

// Configure orientation of the display.
// 0 = none, 1 = 90 degrees clockwise, 2 = 180 degrees, 3 = 270 degrees CW
#define ROTATION     2

//declare microSD variables
#define CHIPSELECT 10
#define ENABLE_DEDICATED_SPI 1
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
#define SPI_DRIVER_SELECT 0
uint8_t cycles = 0;
SdFat32 sd;
File32 measurFile;

unsigned long previousMillis = 0;
unsigned long previousSecond = 0;
unsigned long interval = 100;
float sample_mAs = 0.0;
float total_mAH = 0.0;

void setup() {
  Serial.begin(9600);

  ina219.begin();

  sd.begin(CHIPSELECT);
  measurFile.open("MEAS.csv", O_WRITE | O_CREAT | O_TRUNC);
  measurFile.print("Time,Voltage,Current\n");
  measurFile.sync();

  // Setup the OLED display.
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  Wire.setClock(400000);

  // Clear the display.
  display.clearDisplay();
  display.display();

  // Set rotation.
  display.setRotation(ROTATION);

  // Set text size and color.
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long currentSecond = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    // Read voltage and current from INA219.
    float shuntvoltage = ina219.getShuntVoltage_mV();
    float busvoltage = ina219.getBusVoltage_V();
    float current_mA = ina219.getCurrent_mA();

    // Compute load voltage, power, and milliamp-hours.
    float loadvoltage = busvoltage + (shuntvoltage / 1000);
    float power_mW = loadvoltage * current_mA;

    sample_mAs += current_mA;
    if (currentSecond - previousSecond >= 1000) {
      previousSecond = currentSecond;
      float average_mAs = sample_mAs / 10;
      sample_mAs = 0.0;
      float current_mAH = average_mAs / 3600.0;
      total_mAH += current_mAH;
    }

    updatePowerDisplay(loadvoltage, current_mA, power_mW, total_mAH);
    updateSdCard(currentMillis, loadvoltage, current_mA);
  }
}

void updatePowerDisplay(float loadVoltage, float current_mA, float power_mW, float total_mAH) {
  // Update display.
  display.clearDisplay();
  display.setCursor(0,0);

  // Mode 0, display volts and amps.
  printSIValue(loadVoltage, "V:", 2, 8);
  printDisplayAndSerialChar(' ');
  printSIValue(current_mA/1000.0, "A:", 5, 8);

  display.setCursor(0, 16);
  Serial.print(' ');

  printSIValue(power_mW, "W:", 2, 8);
  printDisplayAndSerialChar(' ');
  printSIValue(total_mAH, "AH:", 5, 8);
  Serial.println(' ');

  display.display();
}

void updateSdCard(unsigned long milliSeconds, float loadVoltage, float current_mA) {
    if (measurFile) {
      measurFile.print(milliSeconds);
      measurFile.print(',');
      measurFile.print(loadVoltage);
      measurFile.print(',');
      measurFile.println(current_mA);

      //after 9 cycles (1 sec.), apply SD buffer changes to file in SD
      if (cycles >= 9) {
        measurFile.sync();
      }

      //increment cycles count + reset to 0 after 10 cycles
      cycles++;
      cycles %= 10;
    }
}

void printSIValue(float value, const char* units, int precision, int maxWidth) {
  // Print a value in SI units with the units left justified and value right justified.
  // Will switch to milli prefix if value is below 1.

  // Add milli prefix if low value.
  if (fabs(value) < 1.0) {
    printDisplayAndSerialChar('m');
    maxWidth -= 1;
    value *= 1000.0;
    precision = max(0, precision-3);
  }

  // Print units.
  printDisplayAndSerialString(units);
  maxWidth -= strlen(units);

  // Leave room for negative sign if value is negative.
  if (value < 0.0) {
    maxWidth -= 1;
  }

  // Find how many digits are in value.
  int digits = ceil(log10(fabs(value)));
  if (fabs(value) < 1.0) {
    digits = 1; // Leave room for 0 when value is below 0.
  }

  // Handle if not enough width to display value, just print dashes.
  if (digits > maxWidth) {
    // Fill width with dashes (and add extra dash for negative values);
    for (int i=0; i < maxWidth; ++i) {
      printDisplayAndSerialChar('-');
    }
    if (value < 0.0) {
      printDisplayAndSerialChar('-');
    }
    return;
  }

  // Compute actual precision for printed value based on space left after
  // printing digits and decimal point.  Clamp within 0 to desired precision.
  int actualPrecision = constrain(maxWidth-digits-1, 0, precision);

  // Compute how much padding to add to right justify.
  int padding = maxWidth-digits-1-actualPrecision;
  for (int i=0; i < padding; ++i) {
    printDisplayAndSerialChar(' ');
  }

  // Finally, print the value!
  display.print(value, actualPrecision);
  Serial.print(value);
}

void printDisplayAndSerialChar(char value) {
  display.print(value);
  Serial.print(value);
}

void printDisplayAndSerialString(const char* value) {
  display.print(value);
  Serial.print(value);
}
