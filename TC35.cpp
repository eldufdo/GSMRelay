#include "TC35.h"
#include "SMS.h"
#include "Arduino.h"

TC35::TC35 () : gsm_serial(TC35_RX,TC35_TX), sms(0,"") {
  gsm_serial.begin(TC35_BAUD);
}

void TC35::sendCommand(String cmd) {
#ifdef TC35_DEBUG
  Serial.print("Sending Cmd: ");
  Serial.println(cmd);
#endif
  gsm_serial.print(cmd);
}


bool TC35::waitForCharacter(char chrToWait,unsigned long timeout) {
  char c = '\0';
  boolean timeoutReached = false;
  unsigned long actTime = millis();
  do {
    if (this->gsm_serial.available() > 0) {
      actTime = millis();
      c = this->gsm_serial.read();
    }
    if ((millis() - actTime > timeout)) {
      timeoutReached = true;
      return false;
    }
  } while (c != chrToWait && timeoutReached == false);
  return true;
}


String TC35::readAnswer(unsigned long timeout,bool checkStartBytes) {
  if (checkStartBytes) {
    if (this->waitForCharacter('\r',timeout) == false) {
  #ifdef TC35_DEBUG
      Serial.println("TIMEOUT bei r");
      return "TIMEOUT";
  #endif
    }
      if (this->waitForCharacter('\n',timeout) == false) {
  #ifdef TC35_DEBUG
      Serial.println("TIMEOUT bei n");
      return "TIMEOUT";
  #endif
    }
  }
  String response = "";
  char chr = '\0';
  char lastChr = '\0';
  unsigned long actTime = millis();
  boolean timeoutReached = false;
  do {
    if (this->gsm_serial.available() > 0) {
      actTime = millis();
      lastChr = chr;
      chr = this->gsm_serial.read();
      response = response + chr;
    }
    if ((millis() - actTime > timeout)) {
      timeoutReached = true;
    }
  } while ( (lastChr != '\r' || chr != '\n') && timeoutReached == false);
  String ret;
  if (timeoutReached == true) {
    ret = "TIMEOUT";
  } else {
    int subStrEnd = response.length()-2;
    if (subStrEnd < 0) {
      subStrEnd = 0;
    }
    ret = response.substring(0,subStrEnd);
  }
#ifdef TC35_DEBUG
  Serial.print("Got response: ");
  Serial.println(ret);
#endif
  return ret;
}




bool TC35::isConnected() {
  this->sendCommand("AT+CREG?\r");
  char c;
  char count = 0;
  this->readAnswer(5000,true);
  this->readAnswer(5000,true);
  return true;
}

bool TC35::setSMSTextMode() {
  this->sendCommand("AT+CMGF=1\r");
  this->readAnswer(5000,true);
  return true;
}


bool TC35::deleteSMS(int index) {
  String cmd = "AT+CMGD=";
  cmd += index;
  cmd += "\r";
  this->sendCommand(cmd);
  String response = "";
  response = this->readAnswer(5000,true);
  return response == "OK";
}


bool TC35::deleteAllSMS() {
  bool error = false;
  for (int i = 1; i <= 20; i++) {
    error = error || !deleteSMS(i);
  }
  return !error;
}

bool TC35::getNewMessage() {
  this->sendCommand("AT+CMGL=\"REC UNREAD\"\r");
  String response = "";
  bool newMsg = false;
  while (response != "OK" && response != "ERROR") {
    response = this->readAnswer(5000,false);
    response.trim();
    if (newMsg == false && response.startsWith("+CMGL: ")) {
      int index = response.substring(7,9).toInt();
      String msg = this->readAnswer(5000,false);
      this->sms.index = index;
      this->sms.msg = msg;
      newMsg = true;
    }    
  }
  return newMsg;
}
