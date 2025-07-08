/*
  serialcomm.h - commnication via serial port

*/
#ifndef Serialcomm_h
#define Serialcomm_h
#include "Arduino.h"
#include "serialcomm.h"
#include "common.h"
#include "utils.h"
namespace serialcomm {

class SerialComm {
private:
  byte receivedBytes[SERIAL_NUM_BYTES];
  bool newData = false;
  byte startMarker = 0x3C; // <
  byte endMarker = 0x3E; // >

public:
  void getBytesFromSerial();
  void showNewData();
};


}

#endif