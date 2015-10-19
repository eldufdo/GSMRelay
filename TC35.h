#include <SoftwareSerial.h>
#include "SMS.h"

#ifndef TC35_H
#define TC35_H

#define TC35_RX 3
#define TC35_TX 2
#define TC35_BAUD 9600

//#define TC35_DEBUG



class TC35 {
   public:
     TC35();
     bool isConnected();
     int getSMSCount();
     void sendCommand(String cmd);
     bool setSMSTextMode();
     bool deleteSMS(int index);
     bool deleteAllSMS();
     bool getNewMessage();
     SMS sms;
   private:
     SoftwareSerial gsm_serial;
     String readAnswer(unsigned long timeOut, bool checkStartBytes);
     bool waitForCharacter(char chrToWait,unsigned long timeout);
};


#endif
