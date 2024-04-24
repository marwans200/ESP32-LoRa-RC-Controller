# ESP32 LoRa RC
Remote control using an ESP32 and a LoRa module.

# Disclaimer! I am a complete beginner and this code might be really not optimized(or whatever the word for it is).

# Important
I discourage anyone doing this project to use an Arduino NANO for the reciever as it is not as powerful as the rest.

I used the ESP32 as the reciever and an Arduino NANO as the transmitter and it still caused issues as it kept restarting sometimes(That could be an error in the code but IDK).

# Setting up the ESP32 transmitter code
Since the ESP32 doesn't support the Servo module, we have to use the ESP32 Servo module to use the RC_ESC module to control any ESC we have. Then, we have to change the #include <Servo.h> in the RC_ESC module to #include <ESP32Servo.h> (or whatever the header file name is). Now you can upload and use the esp32 as a reciever.

I have uploaded the edited code in the ESP32 Transmitter folder.
