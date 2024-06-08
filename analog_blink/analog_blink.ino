// A6, A7 -> new nano
int LED_PIN = A7;
int high = 255;
int low = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  analogWrite(LED_PIN, high);  // turn the LED on (HIGH is the voltage level)
  delay(100);                      // wait for a second
  analogWrite(LED_PIN, low);   // turn the LED off by making the voltage LOW
  delay(100);                      // wait for a second
  analogWrite(LED_PIN, high);  // turn the LED on (HIGH is the voltage level)
  delay(100);  
  analogWrite(LED_PIN, low);   // turn the LED off by making the voltage LOW
  delay(500);  
}