#include <Servo.h>//Using servo library to control ESC

Servo esc; //Creating a servo class with name as esc
long percentage=0;
long x;


float c1;
float c2;
float c3;
float c4;
float temp;
int safety=0;
void setup()

{

esc.attach(8); //Specify the esc signal pin,Here as D8

esc.writeMicroseconds(1000); //initialize the signal to 1000
  pinMode(12,1);
Serial.begin(9600);

}

void loop()

{

int val; //Creating a variable val

val= analogRead(A0); //Read input from analog pin a0 and store in val
x=analogRead(A0);
percentage=(x/1023.)*100.;

Serial.print(percentage);
Serial.print("%");
Serial.print(" --> ");
val= map(val, 0, 1023,1000,2000); //mapping val to minimum and maximum(Change if needed) 

esc.writeMicroseconds(val); //using val as the signal to esc

 temp = analogRead(A4);
  c1 = temp * (5.0 / 1023.0);
  temp = analogRead(A3);
  c2 = temp * (8.4/1023.0)-c1;
  temp = analogRead(A2);
  c3 = temp * (12.6/1023.0)-c2-c1;
  temp = analogRead(A1);
  c4 = temp * (12.6/1023.0)-c3-c2;
  
  Serial.print("bat1:");
  Serial.print(c1);
  Serial.print("  bat2:");
  Serial.print(c2);
  Serial.print("  bat3:");
  Serial.print(c3);
  Serial.print("  bat4:");
  Serial.println(c4);
  delay(2);
  if(c1<=3 || c2<=3 || c3<=3 || c4<=3){
  digitalWrite(12,1);
  delay(20);
   digitalWrite(12,0);
   Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  }
}
