

long x;
//In and Out
int in = PB14;
int in2= PB15;
int in3= PB13;
int take=1;
int data=0;
//Variables
unsigned long duration = 0;
float rpm = 0, val=0, val2=0;
float rpm_a = 0;
int counter = 0;
int present = 0;
int previous = 0;
unsigned long elapsed = 0;
unsigned long elapsed_prev = 0;
int disabled = 0;
int n;
float sum = 0.0, average;






void setup()   {  
  Serial.begin(115200);
  pinMode(in,INPUT);
  pinMode(in2,INPUT);
  pinMode(in3,INPUT);
  // text display tests
  //Serial.println("RPM:");
  //Serial.println(rpm);  
  
  
  elapsed = micros();
}


void loop() {
///////////////////////one rotation measure///////////////////
if (take==1){
  data=digitalRead(in);
}
else {
  data=digitalRead(in2);
}
  
   if (data == 1 && previous == 0)
  {
    previous = 1;
    duration = elapsed - elapsed_prev;
    elapsed_prev  = micros();    
  }
  if (data == 1 && previous == 1)
  {
    previous = 1;       
  }
  if (data == 0 && previous == 1)
  {
    previous = 0;     
  }
  if (data == 0 && previous == 0)
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

  
  if (counter == 50 && take==1)
  {
    //Serial.println("RPM:");
    rpm=sum/50;
    val=rpm;
    sum=0;
    counter = 0;
    take=2;
    rpm=0;
  }
  else if(counter == 50 && take==2){
    rpm=sum/50;
    val2=rpm;
    sum=0;
    counter = 0;
    take=1;
    rpm=0;
  }
}

if (!( (rpm_a-2) < rpm  &&  rpm < (rpm_a+2)))
{
  rpm_a=rpm;
}



if (digitalRead(in3)==1){
  Serial.print(val2);
  Serial.println("!");
}
  else{
  Serial.println(val);}



}//end if pushbutton=1
