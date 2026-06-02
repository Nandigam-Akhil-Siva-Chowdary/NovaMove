/*
 * ESP32 Client (in home)
 * - Receives Bluetooth commands from Server
 * - Controls LED (light) and DC motor (fan)
 */

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;


#define LIGHT_PIN 26
#define FAN_PIN   27

bool lightState = false;
bool fanState   = false;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Client");   

  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  digitalWrite(LIGHT_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);

  Serial.println("Client ESP32 ready - Light & Fan control");
}

void loop() {
  if (SerialBT.available()) {
    byte cmd = SerialBT.read();
    Serial.print("Received command: 0x");
    Serial.println(cmd, HEX);

    switch (cmd) {
      case 0x06:  
        digitalWrite(LIGHT_PIN, HIGH);
        lightState = true;
        Serial.println("Light ON");
        break;

      case 0x07:  
        digitalWrite(LIGHT_PIN, LOW);
        lightState = false;
        Serial.println("Light OFF");
        break;

      case 0x08:  
        digitalWrite(FAN_PIN, HIGH);
        fanState = true;
        Serial.println("Fan ON");
        break;

      case 0x09:  
        digitalWrite(FAN_PIN, LOW);
        fanState = false;
        Serial.println("Fan OFF");
        break;

      default:
        Serial.println("Unknown command (ignored)");
        break;
    }
  }
  delay(10);
}