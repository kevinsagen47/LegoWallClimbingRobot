#include <Servo.h>//Using servo library to control ESC
//#include <VirtualWire.h>
Servo esc1;
Servo esc2;
float valAvg1, valAvg2;
int m1=1040,m2=1040;
int thrust=42;
int thrust2=45;
int uplim=1100;
int lowlim=1005;
int countthrust,state;
int Rrpm1,Rrpm2;
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
esc1.writeMicroseconds(m1);
esc2.writeMicroseconds(m2);


 digitalWrite(9,0);
  delay(5);
 valAvg1=valAvg1+Serial.read();
 digitalWrite(9,1);
  delay(5);
 valAvg2=valAvg2+Serial.read();
 countthrust=countthrust+1;

if(countthrust>=20){
  valAvg1=valAvg1/countthrust;
  Rrpm1=valAvg1;
  valAvg2=valAvg2/countthrust;
  Rrpm2=valAvg2;
if(valAvg1<=thrust2 && m1<=uplim && countthrust>=20){
  m1=m1+1;
  state=1;
}
else if(valAvg1>=thrust && m1>=lowlim && countthrust>=20 || m1>=uplim){
  m1=m1-1;
  state=1;
}

if(valAvg2<=thrust && m2<=uplim && countthrust>=20){
  m2=m2+1;
  state=1;
}
else if(valAvg2>=thrust2 && m2>=lowlim && countthrust>=20|| m2>=uplim){
  m2=m2-1;
  state=1;
}

if(state==1){
  state=0;
  countthrust=0;
}
}
if (m1>=1300 || m2>=1300){
  esc1.writeMicroseconds(1000); //initialize the signal to 1000
esc2.writeMicroseconds(1000);
delay(100000000);
Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!");
}

Serial.print("M1: ");
 Serial.print(Rrpm1);
  Serial.print(" p1: ");
 Serial.print(m1);
 Serial.print(" M2: ");
 Serial.print(Rrpm2);
 Serial.print(" p2: ");
 Serial.println(m2);

}
