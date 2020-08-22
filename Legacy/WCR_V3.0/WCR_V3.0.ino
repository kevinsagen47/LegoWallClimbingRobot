#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo esc1;
Servo esc2;
int delayt=8,mspeed=30000,sswitch=0,ledb=30000,switchdff;//65535max
int pos1=0,pos2=110,pos3=90,pos4=0,pos5=0;
unsigned long time1,time2,time3,time4,time5;
RF24 radio(PA3, PA4);   // nRF24L01 (CE, CSN)PB13,PB12 PB0, PA4
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
unsigned long addtime,looptime;
int startstop=0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit

int uplim=1300;//<------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int lowlim=1005;
int thrust2=65;//upper limit      <------------------------
int thrust=60;//in rpm lower limit<------------------------
int startpoint1=1100,m1=1100;//<---------------------
int startpoint2=1100,m2=1100;//<---------------------
//int in3 = PB13;

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

// Measure the frequency over the specified sample time in milliseconds, returning the frequency in Hz
float readFrequency1(unsigned int sampleTime1)
{
 numPulses1 = 0;                      // prime the system to start a new reading
 attachInterrupt(PB15, isr1, RISING);    // enable the interrupt
 delay(sampleTime1);
 detachInterrupt(0);
 return (numPulses1 < 3) ? 0 : (1000000.0 * (float)(numPulses1 - 2))/(float)(lastPulseTime1 - firstPulseTime1);
}

float readFrequency2(unsigned int sampleTime2)
{
 numPulses2 = 0;                      // prime the system to start a new reading
 attachInterrupt(PB14, isr2, RISING);    // enable the interrupt
 delay(sampleTime2);
 detachInterrupt(0);
 return (numPulses2 < 3) ? 0 : (1000000.0 * (float)(numPulses2 - 2))/(float)(lastPulseTime2 - firstPulseTime2);
}

struct Data_Package {
  short Lx;
  short Ly;
  short Rx;
  short Ry;
  byte b0;//l1
  byte b1;
  byte b2;
  byte b3;
  byte b4;
  byte b5;
  byte b6;//start
  byte b7;//analog
  byte b8;//select
  byte bA1;//R0
};
Data_Package data; //Create a variable with the above structure
void setup() {

  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();

  pinMode(PB6, OUTPUT);
  pinMode(PB7, OUTPUT);
  pinMode(PB8, OUTPUT);
  pinMode(PB9, OUTPUT);
  //pinMode(PB3, OUTPUT);

  pinMode(PC13, OUTPUT);//Green
  pinMode(PC14, OUTPUT);//Blue
  pinMode(PC15, OUTPUT);//Red
  esc1.attach(PB0); 
  esc2.attach(PB1);
  esc1.writeMicroseconds(1000);
  esc2.writeMicroseconds(1000);
}
void loop() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();

  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  Serial.println("COnnection Lost");

  }

  /*
  // Print the data in the Serial Monitor
  Serial.print("Rx: ");
  Serial.print(data.Rx);
  Serial.print("; Ly: ");
  Serial.print(data.Ry);
  Serial.print("Lx: ");
  Serial.print(data.Lx);
  Serial.print("; Ly: ");
  Serial.print(data.Ly);
  Serial.print("; button3: ");
  Serial.print(data.b3);
    Serial.print("; button5: ");
  Serial.print(data.b5);
 Serial.print("; button8: ");
  Serial.print(data.b8);
 Serial.print("; buttonA1: ");
  Serial.print(data.bA1);
  Serial.print("; b2: ");
  Serial.println(data.b2); 
*/
if(data.Ry>1520){
  digitalWrite(PB7,HIGH);
  digitalWrite(PB6,LOW);
}
else if(data.Ry<1480){
  digitalWrite(PB7,LOW);
  digitalWrite(PB6,HIGH);
}

else{
  digitalWrite(PB6,LOW);
  digitalWrite(PB7,LOW);
}
if(data.Ly>1520){
  digitalWrite(PB8,HIGH);
  digitalWrite(PB9,LOW);
}
else if(data.Ly<1480){
  digitalWrite(PB8,LOW);
  digitalWrite(PB9,HIGH);
}

else{
  digitalWrite(PB9,LOW);
  digitalWrite(PB8,LOW);
}



freq1 = readFrequency1(100);//pauses 100ms
 freq2 = readFrequency2(100);//pauses 100ms

 if (startstop==1){

if(freq1>=thrust && freq1<=thrust2 && m1>=lowlim && m1<=uplim){
  m1=m1;
}
else{
if(freq1<=thrust && m1<=uplim){
  m1=m1+10;
}
else if(freq1>=thrust2 && m1>=lowlim|| m1>=uplim){
  m1=m1-3;

}}
if(freq2>=thrust && freq2<=thrust2 && m2>=lowlim && m2<=uplim){
  m2=m2;
}
else{
if(freq2<=thrust && m2<=uplim){
  m2=m2+10;

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
esc1.writeMicroseconds(1000); 
esc2.writeMicroseconds(1000);
digitalWrite(PC15,HIGH);
digitalWrite(PC14,HIGH);
digitalWrite(PC13,HIGH);
Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!");
startstop=0;
}






Serial.print("M1: ");
 Serial.print(freq1);
  Serial.print(" p1: ");
 Serial.print(m1);
 Serial.print(" M2: ");
 Serial.print(freq2);
 Serial.print(" p2: ");
 Serial.println(m2);


if(freq1 <thrust && freq2<thrust){
  digitalWrite(PC13,LOW);
  digitalWrite(PC14,HIGH);

}
else{
  digitalWrite(PC13,HIGH);
  digitalWrite(PC14,LOW);
}



Serial.println(millis()-looptime);
looptime=millis();
}





void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.Lx = 1500; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.Ly = 1500;
  data.Rx = 1500;
  data.Ly = 1500;
  data.b0 = 0;
  data.b1 = 0;
  data.b2 = 0;
  data.b3 = 0;
  data.b4 = 0;
  data.b5 = 0;
  data.b6 = 0;
  data.b7 = 0;
  data.b8 = 0;
  data.bA1 = 0;
}
