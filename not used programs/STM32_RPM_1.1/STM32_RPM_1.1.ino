

long x;
//In and Out
int in3 = PB13;
int in = PB14;
int in2 = PB15;
//Variables
unsigned long duration = 0;
float rpm = 0;
int val=0;
float rpm_a = 0;
int counter = 0;
int present = 0;
int previous = 0;
int take=0;
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
  pinMode(PC13, OUTPUT);
  // text display tests
  //Serial.println("RPM:");
  Serial.println(rpm);  
  
  
  elapsed = micros();
}


void loop() {
///////////////////////one rotation measure///////////////////
//if(take==0){
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
  if (counter == 5)
  {
    //Serial.println("RPM:");
    rpm=sum/5;
    val=rpm;
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
  if (counter2 == 5)
  {
    //Serial.println("RPM:");
    rpm2=sum2/5;
    val2=rpm2;
    take=0;
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
/*
if (digitalRead(in3)==1){
  Serial.write(val2);
 // Serial.println("!");
}
  else{
  Serial.write(val);}
*/
Serial.print("M1: ");
Serial.print(val);
Serial.print("M2: ");
Serial.println(val2);
}//end if pushbutton=1
