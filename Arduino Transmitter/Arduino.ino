#include <SPI.h>
#include <LoRa.h>

//Custom Pins, Change Accordingly
#define LORA_NSS   10
#define LORA_RST   9
#define LORA_DIO0  2
//Analog input Min and Max
#define JoyXMax 60
#define JoyXMin 0
#define JoyYMax 180
#define JoyYMin 0
//My throttle is inverted. Change accordingly
#define ThrottleMax 100
#define ThrottleMin 0

void setup() {
  Serial.begin(9600);
  delay(2500); //Wait to Initialize

  //Set Pin Modes
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);

  //Setting Custom Pins
  LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0); 

  //Begin LoRa. I used 433E6 for the 433MHz range
  if (!LoRa.begin(433E6)) {
      Serial.println("LoRa Initialize failed!");
      while (true);
  }
}

void loop() {
  //Input from Potentiometer for throttle and a Joystick for pitch and roll in my case
  int joyX = analogRead(A0);
  int joyY = analogRead(A1);
  int throttle = analogRead(A2);

  String XtoPWM = String(map(joyX, 0, 900, JoyXMin, JoyXMax));
  String YtoPWM = String(map(joyY,0,900,JoyYMin,JoyYMax));
  int TtoPWM = map(throttle,0,900,ThrottleMin,ThrottleMax);
  String TtoMS = String(map(TtoPWM,ThrottleMax,ThrottleMin,1000,2000));

  String message = XtoPWM + ","+ YtoPWM + "," + TtoMS;
  String analogVals = String(joyX) + ","+ String(joyY) + "," + String(throttle);

  //Debugging
  Serial.println(message);

  //Sending Message
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  //Change or remove accordingly
  delay(50);
}
