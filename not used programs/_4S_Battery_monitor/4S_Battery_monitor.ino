

float c1;
float c2;
float c3;
float c4;
float val;
int safety=0;

void setup() {
  Serial.begin(9600);
  pinMode(12,1);
}

void loop() {
  val = analogRead(A4);
  c1 = val * (5.0 / 1023.0);
  val = analogRead(A3);
  c2 = val * (8.4/1023.0)-c1;
  val = analogRead(A2);
  c3 = val * (12.6/1023.0)-c2-c1;
  val = analogRead(A1);
  c4 = val * (12.6/1023.0)-c3-c2;
  
  Serial.print("cell 1:");
  Serial.print(c1);
  Serial.print("  cell 2:");
  Serial.print(c2);
  Serial.print("  cell 3:");
  Serial.print(c3);
  Serial.print("  cell 4:");
  Serial.println(c4);
  delay(200);
  if(c1<=3.3 || c2<=3.3 || c3<=3.3 || c4<=3.3){
  digitalWrite(12,1);
  delay(20);
   digitalWrite(12,0);
  }
}
