#include "SMS.h"
#include "Arduino.h"

SMS::SMS (int index, String msg) {
  this->index = index;
  this->msg = msg;
}

