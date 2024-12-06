/*
  eeprom.h - Saving configs to Arduino eeprom memory

*/
#ifndef Eeprom_h
#define Eeprom_h
#include <EEPROM.h>
#include "Arduino.h"
#include "utils.h"
#include "common.h"

namespace eeprom {

// Reads saved eeprom bytes and saves them on outputBuffer 
void readCharFromEeprom(int readStart, int readSize, char* outputBuffer);

// Save bytes into eeprom
void saveEeprom(byte string[], size_t arraySize, unsigned int writeStart);

// Save strings (w null termination) to eeprom
void saveStringOnEeprom(byte string[], size_t arraySize, unsigned int writeStart);

// Prints current bytes on eeprom
void printEeprom(int numberOfBytes);


class Eeprom_t {
private:
  uint16_t saved_sequence[CONFIG_SEQUENCE_SIZE];
  char config_vers[CONFIG_VERSION_SIZE] = "";

  // load config from EEPROM and save on currentConfig
  void loadConfig();

  // save config on currentConfig to EEPROM
  void saveConfig(char option[], unsigned int writeStart);

public:
  Eeprom_t() = default;

  // Receives an action
  // - if NEW: save new sequence
  // - if DEF: save default sequence
  void processSequence(char action[3], uint16_t new_sequence[8] = NULL);

  // Returns saved sequence
  void getSequence(uint16_t *sequence);

  // Prints saved sequence
  void printSequence();
};

}

#endif