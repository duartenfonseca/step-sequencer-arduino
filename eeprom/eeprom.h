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

// Read bytes from EEPROM and convert it to a uint16 array
void readU16ArrayFromEeprom(int readStart, int readSize, uint16_t* outputBuffer);

// Save a byte on EEPROM
void saveByteOnEeprom(byte l_byte, unsigned int readPosition);

// Save a uint16 array on EEPROM
void saveU16ArrayOnEeprom(uint16_t* string, size_t arraySize, unsigned int writeStart);

// Save strings (w null termination) to EEPROM
void saveStringOnEeprom(byte string[], size_t arraySize, unsigned int writeStart);

// Prints current bytes on eeprom
void printEeprom(int numberOfBytes);


class Eeprom_t {
private:
  uint16_t savedSequence[CONFIG_SEQUENCE_SIZE];
  char savedAction[CONFIG_ACTION_SIZE] = "";

  // load config from EEPROM and save on currentConfig
  void loadConfig();

  // save config on currentConfig to EEPROM
  void saveConfig(char option[], unsigned int writeStart);

public:
  Eeprom_t(){
    loadConfig();
  }

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