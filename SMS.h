#include "Arduino.h"

#ifndef SMS_H
#define SMS_H



class SMS {
   public:
     SMS(int index,String msg);
     int index;
     String msg;
};


#endif
