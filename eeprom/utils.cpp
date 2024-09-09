#include "utils.h"

void utils::printBin(const byte &aByte) {
  for (int8_t aBit = 7; aBit >= 0; aBit--)
    Serial.write(bitRead(aByte, aBit) ? '1' : '0');
}

void utils::printSequence(const String &instrumentName, byte * instrumentSequence){
  Serial.print(instrumentName + ":     ");
  utils::printBin(instrumentSequence[0]);
  utils::printBin(instrumentSequence[1]);
}
