#include "utils.h"

void  utils::printUint16(const uint16_t &aUint16) {
    utils::processBits(aUint16, [](bool bit, int8_t) {
    Serial.write(bit ? '1' : '0');
  });
}

void utils::printSequence(const uint16_t &instrumentSequence){
  utils::printUint16(instrumentSequence);
  Serial.println();
}

void utils::printSequence(bool instrumentSequence[]){
  for (int i=0; i<16; i++){
    Serial.print(instrumentSequence[i]);
  }  
  Serial.println();
}