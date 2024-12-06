/*
  stepsequencer.h - Step Sequencer configuration

*/
#ifndef Stepsequencer_h
#define Stepsequencer_h
#include "Arduino.h"
#include "utils.h"

namespace stepsequencer {

// stepSequencer_t:
// Saves sequence of each instrument.
// Example:
//
// hiHat:
// 16 bit: 0xFF00
// Sequence: 1111 1111 0000 0000
typedef struct stepSequencer_t {
  uint16_t hiHat;
  uint16_t cymbal;
  uint16_t tomTom;
  uint16_t snare;
  uint16_t bassDrum;
  uint16_t floorTom;
  uint16_t hiHatFoot;
  uint16_t hiHat2;

  stepSequencer_t& operator=(uint16_t a[8]) {
    hiHat = a[0];
    cymbal = a[1];
    tomTom = a[2];
    snare = a[3];
    bassDrum = a[4];
    floorTom = a[5];
    hiHatFoot = a[6];
    hiHat2 = a[7];
    return *this;
  }
  void print();
};

}
#endif