#include <Servo.h>

Servo esc1;
Servo esc2;

int thrust2=70;
int thrust=70;//in rpm
int startpoint1=1050,m1=1050;
int startpoint2=1050,m2=1050;
/*
 int startpoint1=1500,m1=1500;
int startpoint2=1740,m2=1740;
 */
int uplim=1500;//<------
int lowlim=1005;
int countthrust,state; 
float valAvg1, valAvg2;
int Rrpm1,Rrpm2;

long x;
//In and Out
int in3 = PB13;
int in = PB14;
int in2 = PB15;
//Variables
unsigned long duration = 0;
float rpm = 0;
int val1=0;
float rpm_a = 0;
int counter = 0;
int present = 0;
int previous = 0;
int take=0;
int done1=0;
int done2=0;
unsigned long elapsed = 0;
unsigned long elapsed_prev = 0;

//variables2
unsigned long duration2 = 0;
float rpm2 = 0;
int val2=0;
float rpm_a2 = 0;
int counter2 = 0;
int present2 = 0;
int previous2 = 0;
unsigned long elapsed2 = 0;
unsigned long elapsed_prev2 = 0;



//int disabled = 0;
//int n;
float sum = 0.0, average, sum2=0.0;



void setup()   {  
  Serial.begin(115200);
  pinMode(in,INPUT);
  pinMode(in2,INPUT);
  pinMode(in3,INPUT);
  pinMode(PC13, OUTPUT);//Red
  pinMode(PC14, OUTPUT);//Green
  pinMode(PC15, OUTPUT);//Blue
  
  // text display tests
  //Serial.println("RPM:");
  Serial.println(rpm);  
  esc1.attach(PB1); 
  esc2.attach(PB0);
  esc1.writeMicroseconds(1000); //initialize the signal to 1000
esc2.writeMicroseconds(1000);
  elapsed = micros();
}


void loop() {

if(done1==0 || done2==0){
  if (digitalRead(in) == 1 && previous == 0)
  {
    previous = 1;
    duration = elapsed - elapsed_prev;
    elapsed_prev  = micros();    
  }
  if (digitalRead(in) == 1 && previous == 1)
  {
    previous = 1;       
  }
  if (digitalRead(in) == 0 && previous == 1)
  {
    previous = 0;     
  }
  if (digitalRead(in) == 0 && previous == 0)
  {
    previous = 0;
    elapsed = micros();    
  }   
   rpm = 1000000/duration;

if ( (rpm_a-2) < rpm  &&  rpm < (rpm_a+2))
{
  rpm_a = rpm;
  sum=rpm_a+sum;
  counter = counter +1;
  if (counter == 50)
  {
    //Serial.println("RPM:");
    rpm=sum/50;
    val1=rpm;
    done1=1;
    take=1;
    //Serial.println(rpm);  
    sum=0;
    counter = 0;
  }
}

if (!( (rpm_a-2) < rpm  &&  rpm < (rpm_a+2)))
{
  rpm_a=rpm;
}
//}

//else{
//sensor 2/////////////////////////////////////////////////////
//if(take==1){
  if (digitalRead(in2) == 1 && previous2 == 0)
  {
    previous2 = 1;
    duration2 = elapsed2 - elapsed_prev2;
    elapsed_prev2  = micros();    
  }
  if (digitalRead(in2) == 1 && previous2 == 1)
  {
    previous2 = 1;       
  }
  if (digitalRead(in2) == 0 && previous2 == 1)
  {
    previous2 = 0;     
  }
  if (digitalRead(in2) == 0 && previous2 == 0)
  {
    previous2 = 0;
    elapsed2 = micros();    
  }   
   rpm2 = 1000000/duration2;

if ( (rpm_a2-2) < rpm2  &&  rpm2 < (rpm_a2+2))
{
  rpm_a2 = rpm2;
  sum2=rpm_a2+sum2;
  counter2 = counter2 +1;
  if (counter2 == 50)
  {
    //Serial.println("RPM:");
    rpm2=sum2/50;
    val2=rpm2;
    take=0;
    done2=1;
    //Serial.println(rpm);  
    sum2=0;
    counter2 = 0;
  }
}

if (!( (rpm_a2-2) < rpm2  &&  rpm2 < (rpm_a2+2)))
{
  rpm_a2=rpm2;
 }
//}

if (!( (rpm_a2-2) < rpm2  &&  rpm2 < (rpm_a2+2)))
{
  rpm_a2=rpm2;
}
//}

}

///*
if(done1==1 && done2==1){
if (digitalRead(in3)==1){
esc1.writeMicroseconds(m1);
esc2.writeMicroseconds(m2);
 valAvg1=valAvg1+val2;
 valAvg2=valAvg2+val1;
 countthrust=countthrust+1;

if(countthrust>=50){
  valAvg1=valAvg1/countthrust;
  Rrpm1=valAvg1;
  valAvg2=valAvg2/countthrust;
  Rrpm2=valAvg2;
if(valAvg1<=thrust2 && m1<=uplim && countthrust>=20){
  m1=m1+3;
  state=1;
}
else if(valAvg1>=thrust && m1>=lowlim && countthrust>=20 || m1>=uplim){
  m1=m1-3;
  state=1;
}

if(valAvg2<=thrust && m2<=uplim && countthrust>=20){
  m2=m2+3;
  state=1;
}
else if(valAvg2>=thrust2 && m2>=lowlim && countthrust>=20|| m2>=uplim){
  m2=m2-3;
  state=1;
}

if(state==1){
  state=0;
  countthrust=0;
}
}
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
 Serial.print(val1);
  Serial.print(" p1: ");
 Serial.print(m1);
 Serial.print(" M2: ");
 Serial.print(val2);
 Serial.print(" p2: ");
 Serial.println(m2);




done1=0;
done2=0;


if(val1 <thrust || val2<thrust){
  digitalWrite(PC13,HIGH);
  digitalWrite(PC14,LOW);
}
else{
  digitalWrite(PC13,LOW);
  digitalWrite(PC14,HIGH);
}
}/*
Serial.print("M1: ");
Serial.print(val1);
Serial.print("M2: ");
Serial.println(val2);
*/






}//end if pushbutton=1
