/*
  stepsequencer.h - Step Sequencer configuration

*/
#ifndef Stepsequencer_h
#define Stepsequencer_h
#include "Arduino.h"
#include "utils.h"
#include "common.h"

namespace stepsequencer {

// stepSequencer_t:
// Saves sequence of each instrument.
// Example:
//
// hiHat:
// 16 bit: 0xFF00
// Sequence: 1111 1111 0000 0000

typedef struct stepSequencer_t {
  bool hiHat[NUM_STEPS]{DEFAULT_SEQUENCE};
  bool cymbal[NUM_STEPS]{DEFAULT_SEQUENCE};
  bool tomTom[NUM_STEPS]{DEFAULT_SEQUENCE};
  bool snare[NUM_STEPS]{DEFAULT_SEQUENCE};
  bool bassDrum[NUM_STEPS]{DEFAULT_SEQUENCE};
  bool floorTom[NUM_STEPS]{DEFAULT_SEQUENCE};
  bool hiHatFoot[NUM_STEPS]{DEFAULT_SEQUENCE};
  bool hiHat2[NUM_STEPS]{DEFAULT_SEQUENCE};

  // Save a sequence to the step sequence struct
  void saveSequence(uint16_t sequence[CONFIG_NUM_INSTRUMENTS]);

  // Convert a sequence as uint16 to a bool array
  void convertSequenceU16ToBool(const uint16_t &newInstrumentSequence, bool instrument[]);

  // Print current saved sequence
  void printSequence();
};

}
#endif