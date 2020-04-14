  // Arduino code is available to download - link below the video.

//Connect the Receiver data pin to Arduino pin 8

#include <VirtualWire.h>
int left=0;
int right=0;
int stat=0;
int M_Speed=255;
int trig=0;
int thrust=35;
byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message

void setup() {
 Serial.begin(9600);
 Serial.println("Device is ready");
// Initialize the IO and ISR
 vw_setup(2000); // Bits per sec
 vw_rx_start(); // Start the receiver
 pinMode(3,1);
 pinMode(5,1);
 pinMode(6,1);
 pinMode(10,1);
 pinMode(9,1);
}

void loop() {
  //Serial.write(thrust);
  if (vw_get_message(message, &messageLength)) {  // Non-blocking
 
     Serial.write(message[0]);
     Serial.write(message[1]);
    Serial.println("");
    //Serial.println("__________");
 left=message[0]-48;
 right =message[1]-48;
if(left>=3){
  left=left-3;
  trig=1;
}
else{
  trig=0;
}
 stat=trig*100+right*10+left;
//Serial.println(stat);

if(trig==1){
  digitalWrite(9,1);
}
else{
  digitalWrite(9,0);
}
 if (right==1)
 {
  analogWrite(3,M_Speed);
  analogWrite(5, 0);
 }
 else if (right==2)
 {
  digitalWrite(5, M_Speed);
    analogWrite(3, 0);

 }
 else
 {
  analogWrite(3,0);
  analogWrite(5,0);
 }




 if (left==2)
 {
  analogWrite(10, M_Speed);
    analogWrite(6,0);

 }
 else if (left==1)
 {
  analogWrite(6, M_Speed);
  analogWrite(10,0);
 }
 else
 {
  analogWrite(6,0);
  analogWrite(10,0);
 }
right =0;
left=0;
}/*
else{
 digitalWrite(3,0);
 digitalWrite(5,0);
 digitalWrite(6,0);
 digitalWrite(10,0);
}*/
/*
digitalWrite(3,0);
digitalWrite(5,0);
digitalWrite(6,0);
digitalWrite(10,0);
*/
}
