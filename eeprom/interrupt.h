/*
  interrupt.h - arduino interrupt related functions

*/
#ifndef Interrupt_h
#define Interrupt_h
#include "Arduino.h"
#include "common.h"
namespace interrupt {

// Get the OCR1A value from the number of milliseconds for the loop
uint16_t msToOcr1a(const int &milliseconds);

class interruptCycle {
public:
  int stepCounter;
  bool stepCycle;

  interruptCycle(){
    stepCounter = 0;
    stepCycle = false;
  }
  void setupInterrupt(const int &frequencyMs);


};


}

#endif