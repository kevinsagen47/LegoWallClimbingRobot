
// Frequency counter sketch, for measuring frequencies low enough to execute an interrupt for each cycle
// Connect the frequency source to the INT0 pin (digital pin 2 on an Arduino Uno)

#include <Servo.h>

Servo esc1;
Servo esc2; 
int uplim=1500;//<------
int lowlim=1005;
int thrust2=65;//upper limit
int thrust=60;//in rpm  lower limit
int startpoint1=1080,m1=1080;
int startpoint2=1080,m2 =1080;
int in3 = PB13;

volatile unsigned long firstPulseTime1;
volatile unsigned long lastPulseTime1;
volatile unsigned long numPulses1;

volatile unsigned long firstPulseTime2;
volatile unsigned long lastPulseTime2;
volatile unsigned long numPulses2;

float freq;
float freq1;
float freq2;
int mode=0;

void isr1()
{
 unsigned long now = micros();
 if (numPulses1 == 1)
 {
   firstPulseTime1 = now;
 }
 else 
 {
   lastPulseTime1 = now;
 }
 ++numPulses1;
}

void isr2()
{
 unsigned long now2 = micros();
 if (numPulses2 == 1)
 {
   firstPulseTime2 = now2;
 }
 else
 {
   lastPulseTime2 = now2;
 }
 ++numPulses2;
}

void setup()
{
 Serial.begin(115200);    // this is here so that we can print the result
 pinMode(in3,INPUT);
  pinMode(PC13, OUTPUT);//Red
  pinMode(PC14, OUTPUT);//Green
  pinMode(PC15, OUTPUT);//Blue
  esc1.attach(PB0); 
  esc2.attach(PB1);
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000);
}

// Measure the frequency over the specified sample time in milliseconds, returning the frequency in Hz
float readFrequency1(unsigned int sampleTime1)
{
 numPulses1 = 0;                      // prime the system to start a new reading
 attachInterrupt(PB14, isr1, RISING);    // enable the interrupt
 delay(sampleTime1);
 detachInterrupt(0);
 return (numPulses1 < 3) ? 0 : (1000000.0 * (float)(numPulses1 - 2))/(float)(lastPulseTime1 - firstPulseTime1);
}

float readFrequency2(unsigned int sampleTime2)
{
 numPulses2 = 0;                      // prime the system to start a new reading
 attachInterrupt(PB15, isr2, RISING);    // enable the interrupt
 delay(sampleTime2);
 detachInterrupt(0);
 return (numPulses2 < 3) ? 0 : (1000000.0 * (float)(numPulses2 - 2))/(float)(lastPulseTime2 - firstPulseTime2);
}


void loop(){
 freq1 = readFrequency1(100);
 freq2 = readFrequency2(100);

 if (digitalRead(in3)==1){

if(freq1>=thrust && freq1<=thrust2 && m1>=lowlim && m1<=uplim){
  m1=m1;
}
else{
if(freq1<=thrust && m1<=uplim){
  m1=m1+3;
}
else if(freq1>=thrust2 && m1>=lowlim|| m1>=uplim){
  m1=m1-3;

}}
if(freq2>=thrust && freq2<=thrust2 && m2>=lowlim && m2<=uplim){
  m2=m2;
}
else{
if(freq2<=thrust && m2<=uplim){
  m2=m2+3;

}
else if(freq2>=thrust2 && m2>=lowlim|| m2>=uplim){
  m2=m2-3;
}

}
esc1.writeMicroseconds(m1);
esc2.writeMicroseconds(m2);
}


else{
  esc1.writeMicroseconds(1000);
esc2.writeMicroseconds(1000);
m1=startpoint1;
m2=startpoint2;
}

if (m1>=uplim || m2>=uplim){
  esc1.writeMicroseconds(1000); //initialize the signal to 1000
esc2.writeMicroseconds(1000);
digitalWrite(PC15,HIGH);
delay(100000000);
Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!");
}






Serial.print("M1: ");
 Serial.print(freq1);
  Serial.print(" p1: ");
 Serial.print(m1);
 Serial.print(" M2: ");
 Serial.print(freq2);
 Serial.print(" p2: ");
 Serial.println(m2);


if(freq1 <thrust || freq2<thrust){
  digitalWrite(PC13,HIGH);
  digitalWrite(PC14,LOW);
}
else{
  digitalWrite(PC13,LOW);
  digitalWrite(PC14,HIGH);
}
}
