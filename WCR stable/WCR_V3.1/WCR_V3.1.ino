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
unsigned long addtime,looptime, delaym;
int startstop=0;
int delaymv=200;
long ffreq1,ffreq2,timerr1,timerr2;

int flag1,flag2;
// Max size of this struct is 32 bytes - NRF24L01 buffer limit

int uplim=2000;//<------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int lowlim=1013;
int thrust2=355;//upper limit      <------------------------
int thrust=350;//in rpm lower limit<------------------------
int startpoint1=1405,m1=1405;//<---------------------
int startpoint2=1405,m2=1405;//<---------------------
//int in3 = PB13;

float freq;
long freq1;long freq2;
int mode=0;

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
  pinMode(PA0, INPUT);

  // stop the timers before configuring them
  Timer2.pause();

  Timer2.setPrescaleFactor(72); // 1 microsecond resolution

  // setup timer 2 channel 1 capture on rising edge
  Timer2.setInputCaptureMode(TIMER_CH1, TIMER_IC_INPUT_DEFAULT); // use default input TI1
  // setup timer 2 channel 2 capture on falling edge
  Timer2.setInputCaptureMode(TIMER_CH2, TIMER_IC_INPUT_SWITCH); // use switched input TI1
  Timer2.setPolarity(TIMER_CH2, 1); // trigger on falling edge

  // counter setup as slave triggered by TI1 in reset mode
  Timer2.setSlaveFlags( TIMER_SMCR_TS_TI1FP1 | TIMER_SMCR_SMS_RESET );

  Timer2.refresh();
  Timer2.getCompare(TIMER_CH1); // clear capture flag
  Timer2.getCompare(TIMER_CH2); // clear capture flag
  Timer2.resume(); // let timer 2 run


  pinMode(PA8, INPUT);
 // stop the timers before configuring them
 Timer1.pause();

  Timer1.setPrescaleFactor(72); // 1 microsecond resolution

  // setup timer 2 channel 1 capture on rising edge
  Timer1.setInputCaptureMode(TIMER_CH1, TIMER_IC_INPUT_DEFAULT); // use default input TI1
  // setup timer 2 channel 2 capture on falling edge
  Timer1.setInputCaptureMode(TIMER_CH2, TIMER_IC_INPUT_SWITCH); // use switched input TI1
  Timer1.setPolarity(TIMER_CH2, 1); // trigger on falling edge

  // counter setup as slave triggered by TI1 in reset mode
  Timer1.setSlaveFlags( TIMER_SMCR_TS_TI1FP1 | TIMER_SMCR_SMS_RESET );

  Timer1.refresh();
  Timer1.getCompare(TIMER_CH1); // clear capture flag
  Timer1.getCompare(TIMER_CH2); // clear capture flag
  Timer1.resume(); // let timer 2 run
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

  legomotor();
  calculatefreq();
  mainmotor();


/*

Serial.print("Rx: ");
  Serial.print(data.Rx);
  Serial.print("; Ly: ");
  Serial.print(data.Ry);
  Serial.print("Lx: ");
  Serial.print(data.Lx);
  Serial.print("; Ly: ");
  Serial.print(data.Ly);
  Serial.print("; button3: ");
  Serial.println(data.b3);
  */

  Serial.print("M1: ");
 Serial.print(freq1);
  Serial.print(" p1: ");
 Serial.print(m1);
 Serial.print(" M2: ");
 Serial.print(freq2);
 Serial.print(" p2: ");
 Serial.println(m2);

if(startstop){
if(freq1 <thrust && freq2<thrust){
  digitalWrite(PC13,LOW);
  digitalWrite(PC14,HIGH);

}
else{
  digitalWrite(PC13,HIGH);
  digitalWrite(PC14,LOW);
}}
else
{
    digitalWrite(PC13,LOW);
  digitalWrite(PC14,LOW);
}

}



void legomotor(){

  
  if(data.Ry>1520){
  digitalWrite(PB7,HIGH);
  digitalWrite(PB6,LOW);
}
else if(data.Ry<1480){
  digitalWrite(PB7,LOW);
  digitalWrite(PB6,HIGH);
  Serial.println("true");
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
}

void mainmotor(){

if(millis()-delaym>=delaymv){
if(data.b3==1)startstop=1;
if(data.b2==1)startstop=0;


  
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
delaym=millis();}

}

void calculatefreq(){
 if(Timer2.getInputCaptureFlag(TIMER_CH1))
{timerr1=millis();flag1=0;}
if(millis()-timerr1>1000)flag1=1;
ffreq2=1000000/Timer2.getCompare(TIMER_CH1);
if(flag1==0)freq2=ffreq2;
else freq2=0;

if(Timer1.getInputCaptureFlag(TIMER_CH1))
{timerr2=millis();flag2=0;}
if(millis()-timerr2>1000)flag2=1;
ffreq1=1000000/Timer1.getCompare(TIMER_CH1);
if(flag2==0)freq1=ffreq1;
else freq1=0;
}

void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.Lx = 1500; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.Ly = 1500;
  data.Rx = 1500;
  data.Ry = 1500;
  data.b0 = 0;
  data.b1 = 0;
  data.b2 = 1;
  data.b3 = 0;
  data.b4 = 0;
  data.b5 = 0;
  data.b6 = 0;
  data.b7 = 0;
  data.b8 = 0;
  data.bA1 = 0;
}
