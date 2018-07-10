/*
 *
 *  Default Connection:
 *  GSM RX --> Arduino Digital 3
 *  GSM TX --> Arduino Digital 2
 *  Relais IN1 --> Arduino Digital 8
 *  Relais IN2 --> Arduino Digital 8
 *
 */

#include <SoftwareSerial.h>
#include "SMS.h"
#include "TC35.h"

#define RELAY1 8
#define RELAY2 9

#define RELAY_OPEN HIGH
#define RELAY_CLOSED LOW

TC35 tc35;

void setup() {
  Serial.begin(9600);
  Serial.println("GSM Relay Switcher");
  Serial.println("Waiting until TC35 connects to the mobile network");
  while (tc35.isConnected() == false) {
      delay(1000);
  }
  Serial.println("Successfully connected to the mobile network");
  pinMode(RELAY1,OUTPUT);
  pinMode(RELAY2,OUTPUT);
  digitalWrite(RELAY1,HIGH);
  digitalWrite(RELAY2,HIGH);
  tc35.setSMSTextMode();
  Serial.print("Deleting all stored SMS ...");
  tc35.deleteAllSMS();
  Serial.println("Done");
  Serial.println("Waiting for SMS...");
}

void loop() {
  while (tc35.isConnected() == false) {
      delay(1000);
  }
  if (tc35.getNewMessage()) {
    tc35.sms.msg.toLowerCase();
    if (tc35.sms.msg == "a") {
      digitalWrite(RELAY1,RELAY_CLOSED);
      delay(1000);
      digitalWrite(RELAY1,RELAY_OPEN);
      delay(1000);
    } else if (tc35.sms.msg == "z") {
      digitalWrite(RELAY2,RELAY_CLOSED);
      delay(1000);
      digitalWrite(RELAY2,RELAY_OPEN);
      delay(1000);
    }
    tc35.deleteSMS(tc35.sms.index);
  }
  delay(1000);
}


