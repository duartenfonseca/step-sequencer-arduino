#include "stepsequencer.h"
namespace stepsequencer {

void stepSequencer_t::printSequence(){
  Serial.println("///////////// PRINTING CURRENT SEQUENCE \\\\\\\\\\\\\\\\\\");
  utils::printSequence(hiHat);
  utils::printSequence(cymbal);
  utils::printSequence(tomTom);
  utils::printSequence(snare);
  utils::printSequence(bassDrum);
  utils::printSequence(floorTom);
  utils::printSequence(hiHatFoot);
  utils::printSequence(hiHat2);
}

void stepSequencer_t::saveSequence(uint16_t sequence[8])
  {
    convertSequenceU16ToBool(sequence[0],hiHat);
    convertSequenceU16ToBool(sequence[1],cymbal);
    convertSequenceU16ToBool(sequence[2],tomTom);
    convertSequenceU16ToBool(sequence[3],snare);
    convertSequenceU16ToBool(sequence[4],bassDrum);
    convertSequenceU16ToBool(sequence[5],floorTom);
    convertSequenceU16ToBool(sequence[6],hiHatFoot);
    convertSequenceU16ToBool(sequence[7],hiHat2);
  }

void stepSequencer_t::convertSequenceU16ToBool(const uint16_t &newInstrumentSequence, bool instrument[]){
  utils::processBits(newInstrumentSequence, [&](bool bit, uint8_t pos) {
    instrument[pos] = bit;
  });
}
}