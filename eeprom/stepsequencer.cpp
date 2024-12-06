#include "stepsequencer.h"
namespace stepsequencer {

void stepSequencer_t::print(){
  Serial.println("///////////// PRINTING CONFIG \\\\\\\\\\\\\\\\\\");
  utils::printSequence(hiHat);
  utils::printSequence(cymbal);
  utils::printSequence(tomTom);
  utils::printSequence(snare);
  utils::printSequence(bassDrum);
  utils::printSequence(floorTom);
  utils::printSequence(hiHatFoot);
  utils::printSequence(hiHat2);
  // Serial.print("//////////////////////  \\\\\\\\\\\\\\\\\\\\\\\\\\");
}
}