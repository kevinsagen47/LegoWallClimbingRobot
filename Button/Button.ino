/*
 Button

 Turns on and off the built-in LED when the built-in button is
 pressed.

 Ported to Maple from the Arduino example 27 May 2011
 by Marti Bolivar
*/
#define BOARD_BUTTON_PIN PB9
void setup() {
  // Initialize the built-in LED pin as an output:
  pinMode(PC13, OUTPUT);
  // Initialize the built-in button (labeled BUT) as an input:
  pinMode(BOARD_BUTTON_PIN, INPUT);
}



void loop() {
    // Check if the button is pressed.
    if(digitalRead(PB9))digitalWrite(PC13,HIGH);
    else digitalWrite(PC13,LOW);
    Serial.println(digitalRead(PB9));
}
