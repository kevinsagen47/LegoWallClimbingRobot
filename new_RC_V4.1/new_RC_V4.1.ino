#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(PB0, PA4);   // nRF24L01 (CE, CSN)PB13,PB12 PB0, PA4
const byte sendaddress[6] = "11011";
const byte address[6] = "00100";//new rc
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
unsigned long freetime;
int y,x;
int lcdcount=1;
int prev1,prev2,prev3,prev4,prev5,prev6;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);


#define outputA PA8
 #define outputB PA9
 int counter =30; 
 int aState;
 int aLastState;  


// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Send_Package {
  short bat1;
  short bat2;
  short freq1;//l1
  short freq2;
  short m1;
  short m2;
  byte stat;

};
Send_Package Send; //Create a variable with the above structure
struct Data_Package {
  short x;
  short y;
  byte b0;//l1
  byte throttle;

};
Data_Package data;

void setup() {
  pinMode (PB9,INPUT);
  pinMode (PB8,INPUT);
  pinMode (outputA,INPUT);
   pinMode (outputB,INPUT);
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(1, sendaddress);
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
  aLastState = digitalRead(outputA);
  //pinMode(PB3, OUTPUT);

  lcd.begin();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("Wall Climbing Rob");
lcd.setCursor(0,1);
lcd.print(" Remote Controller ");
}
void loop() {
    radio.startListening();
  // Check whether there is data to be received
  freetime=millis();
  while (!radio.available()&&millis()-freetime<=100);//
  if (radio.available()) {
    radio.read(&Send, sizeof(Send_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();

  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  Serial.println("COnnection Lost");}

  
  radio.stopListening();
  y=analogRead(PA0);
  x=analogRead(PA1);
  data.y=map(y,0,4095,1000,2000);
  data.x=map(x,0,4095,1000,2000);
  data.b0=digitalRead(PB9);
  data.throttle=counter;
  radio.write(&data, sizeof(Data_Package));


 while(digitalRead(PB8)){
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter=counter+5;
     } else {
       counter=counter-5;
     }
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   aLastState = aState;
   data.throttle=counter;
  // lcd.clear();
 lcd.setCursor(10,1);
  lcd.print(data.throttle);
   radio.write(&data, sizeof(Data_Package));
 }
 //lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("                    ");
  lcd.setCursor(0,0);
  lcd.print(Send.m1);
  lcd.setCursor(5,0);
  lcd.print(Send.m2);
  lcd.setCursor(10,0);
  lcd.print(digitalRead(PB9));

  
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print(Send.freq1);
  lcd.setCursor(5,1);
  lcd.print(Send.freq2);
  lcd.setCursor(10,1);
  lcd.print(data.throttle);
  /*
  short roll_angle;
  short pitch_angle;
  short bat1;
  short bat2;
  byte rssi;
  byte flight_mode;
  byte altitude_meters;
  byte error;
  byte number_used_sats;
  int l_lat_gps;
  int l_lon_gps;
  byte start;
   */
  // Print the data in the Serial Monitor
  Serial.print("bat1: ");
  Serial.print(Send.bat1);
  Serial.print("bat2: ");
  Serial.print(Send.bat2);
  Serial.print("; freq1: ");
  Serial.print(Send.freq1);
  Serial.print("; freq2: ");
  Serial.print(Send.freq2);
  Serial.print("; m1: ");
  Serial.print(Send.m1); 
  Serial.print("; m2: ");
  Serial.print(Send.m2);
  Serial.print("; stat: ");
  Serial.println(Send.stat);
  

}


//Channel 1 => Roll     => data.Rx
//Channel 2 => pitch    => data.Ry
//Channel 3 => throttle => data.Ly
//Channel 4 => yaw      => data.Lx


void resetData() {

}
