#include <SPI.h>
#include <LoRa.h>
#include <ESP32Servo.h>
#include <ESC.h>

//Define Custom Pins
#define LORA_NSS   5
#define LORA_RST    2
#define LORA_DIO0   4

//Define the max Servo rotation per message recieved. Scroll below to the split string code to know why.
#define MaxRot 8

//Defining Servos and ESC
Servo Servo1;
Servo Servo2;
Servo Servo3;

//Initializing the ESC
ESC myESC(13,1000,2000,900);

//int array to store the positional value recieved from the transmitter
int pos[3];

void setup() {
  Serial.begin(9600);
  delay(2500); // Wait to initialize

  //Attaching Servos to pins and arming the BLDC motor...
  myESC.arm();
  Servo1.attach(26);
  Servo2.attach(14);
  Servo3.attach(27);

  //Setting Custom Pins...
  LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Failed to Load!");
    while (1);
  }

}

void loop() {
  //Checking if meesage was sent
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    
    String msg = "";
    while (LoRa.available()) {
      msg += (char)LoRa.read();
    }

    splitString(msg,pos);

    //Writing the position of the Servos... You can move it inside the if statement above to not waste cpu cycles or whatever idk
    Servo1.write(pos[0]);
    Servo2.write(pos[1]);
    Servo3.write(pos[1]);

    Serial.println(msg);
    
  }

  //Writing ESC Speed... Same as above
  myESC.speed(pos[2]);
}

//A whole ass code for spliting string coz I don't actually know how to
void splitString(String input, int arr[]) {

  int commaIndex = 0;
  int nextCommaIndex = 0;

  for (int i = 0; i < 3; i++) {
    // Find the index of the next comma
    nextCommaIndex = input.indexOf(',', commaIndex);

    // Extract the substring and convert it to an integer
    String substring;
    if (nextCommaIndex != -1) {
      substring = input.substring(commaIndex, nextCommaIndex);
    } else {
      // If no more commas, take the substring until the end
      substring = input.substring(commaIndex);
    }

    //The value returned after splitting
    int vals = substring.toInt();

    //Checking value before applying because moving the servos to an angle a bit far can cause it to draw excess current and cause the ESP32 to Restart.
    //Change the max value accordingly.
    if(vals >= arr[i] + MaxRot){
      arr[i] += MaxRot;
    }
    else if(vals <= arr[i] - MaxRot){
      arr[i] -= MaxRot;
    }
    else{
      arr[i] = vals;
    }

    if(i == 2) arr[i] = vals;
    // Move the index past the comma
    commaIndex = nextCommaIndex + 1;
  }
}
