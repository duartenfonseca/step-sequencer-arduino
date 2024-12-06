#include "utils.h"

void utils::printBin(const byte &aByte) {
  for (int8_t aBit = 7; aBit >= 0; aBit--)
    Serial.write(bitRead(aByte, aBit) ? '1' : '0');
}

void utils::printSequence(uint16_t instrumentSequence){
  utils::printBin(instrumentSequence >> 8);
  utils::printBin(instrumentSequence & 0xff);
  Serial.println();
}
