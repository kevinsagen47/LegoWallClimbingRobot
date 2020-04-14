

long x;
//In and Out
int in = PB14;
//Variables
unsigned long duration = 0;
float rpm = 0;
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
  // text display tests
  //Serial.println("RPM:");
  Serial.println(rpm);  
  
  
  elapsed = micros();
}


void loop() {
///////////////////////one rotation measure///////////////////
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
    Serial.println(rpm);  
    sum=0;
    counter = 0;
  }
}

if (!( (rpm_a-2) < rpm  &&  rpm < (rpm_a+2)))
{
  rpm_a=rpm;
}







}//end if pushbutton=1
