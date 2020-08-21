#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(PB0, PA4);   // nRF24L01 (CE, CSN)PB13,PB12 PB0, PA4
const byte sendaddress[6] = "11011";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
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
void setup() {


  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, sendaddress);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
  pinMode(PC13, OUTPUT);
  //pinMode(PB3, OUTPUT);
}
void loop() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&Send, sizeof(Send_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();

  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  Serial.println("COnnection Lost");

  
  }

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
