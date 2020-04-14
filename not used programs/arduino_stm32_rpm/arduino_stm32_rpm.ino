#include <Servo.h>//Using servo library to control ESC
//#include <VirtualWire.h>
Servo esc1;
Servo esc2;
float val1, val2;
int m1,m2;
int thrust=50;
int uplim=400;
int lowlim=5;
//byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
//byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message


void setup() {
  
  esc1.attach(4); //Specify the esc signal pin,Here as D8
  esc2.attach(7);
esc1.writeMicroseconds(1000); //initialize the signal to 1000
esc2.writeMicroseconds(1000);
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(9,1);
delay(5000);
//vw_setup(2000); // Bits per sec
 //vw_rx_start(); // Start the receiver
}

void loop() {
  // put your main code here, to run repeatedly:
//if (vw_get_message(message, &messageLength)) {
  
//}
 digitalWrite(9,0);
  delay(5);
 val2=Serial.read();
 digitalWrite(9,1);
  delay(5);
 val1=Serial.read();
 Serial.print("M1: ");
 Serial.print(val1);
 Serial.print("M2: ");
 Serial.println(val2);




}
