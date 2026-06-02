/*
 * ESP32 Server (on wheelchair)
 * - Reads VC-02 commands from UART2
 * - Forwards 0x06..0x09 to Client via Bluetooth
 * - Directly controls two stepper motors for 0x0A (sleep) and 0x0B (sit)
 */

#include "BluetoothSerial.h"
#include <Stepper.h>

BluetoothSerial SerialBT;
String clientName = "ESP32_Client";   
bool connected = false;

const int stepsPerRevolution = 2048; 

Stepper backrestMotor(stepsPerRevolution, 18, 19, 21, 22);
Stepper seatMotor(stepsPerRevolution, 25, 26, 32, 33);

const int backrestSleepSteps = 512;   
const int backrestSitSteps   = -512;  
const int seatAdjustSteps    = 256;   

void setup() {
  Serial.begin(115200);          
  Serial2.begin(115200);        


  backrestMotor.setSpeed(15);
  seatMotor.setSpeed(15);

  SerialBT.begin("ESP32_Server", true);  
  connected = SerialBT.connect(clientName);
  if (connected) {
    Serial.println("Connected to Client ESP32");
  } else {
    Serial.println("Failed to connect to Client. Light/Fan commands will be ignored.");
  }

  Serial.println("Server ESP32 ready");
}

void loop() {
  if (Serial2.available()) {
    byte cmd = Serial2.read();
    Serial.print("Received command: 0x");
    Serial.println(cmd, HEX);

    switch (cmd) {
      case 0x06:  
      case 0x07:  
      case 0x08:   
      case 0x09:   
        if (connected) {
          SerialBT.write(cmd);
          Serial.println("Forwarded to Client");
        } else {
          Serial.println("Client not connected");
        }
        break;

      case 0x0A: 
        Serial.println("Moving backrest to SLEEP position");
        backrestMotor.step(backrestSleepSteps);
        seatMotor.step(seatAdjustSteps);
        break;

      case 0x0B:   
        Serial.println("Moving backrest to SIT position");
        backrestMotor.step(backrestSitSteps);
        seatMotor.step(-seatAdjustSteps);
        break;

      default:
        break;
    }
  }
  delay(10);
}