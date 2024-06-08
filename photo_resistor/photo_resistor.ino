//Constants
const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int ledPin = 2;       // Led pin at Arduino pin 9

//Variables
int value;				  // Store value from photoresistor (0-1023)

void setup(){
 pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
 pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
}

void loop() {
  value = analogRead(pResistor);
  if (value > 500) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
