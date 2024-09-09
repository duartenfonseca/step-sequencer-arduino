/*
  eeprom.h - Saving configs to Arduino eeprom battery

*/
#ifndef Eeprom_h
#define Eeprom_h
#include <EEPROM.h>
#include "Arduino.h"
#include "utils.h"
#include "common.h"

namespace eeprom {

// Reads saved eeprom bytes and saves them on outputBuffer 
void readEeprom(int readStart, int readSize, char* outputBuffer);

// Save bytes into eeprom
void saveEeprom(byte string[], size_t arraySize, unsigned int writeStart);

// Save strings (w null termination) to eeprom
void saveStringOnEeprom(byte string[], size_t arraySize, unsigned int writeStart);

// Prints current bytes on eeprom
void printEeprom(int numberOfBytes);

// stepSequencer_t:
// Saves sequence of each instrument.
// Example:
//
// hiHat:
// Byte 0: 0xff
// Byte 1: 0x00
// Sequence: 1111 1111 0000 0000
typedef struct stepSequencer_t {
  byte hiHat[2];
  byte cymbal[2];
  byte tomTom[2];
  byte snare[2];
  byte bassDrum[2];
  byte floorTom[2];
  byte hiHatFoot[2];
  byte hiHat2[2];

  stepSequencer_t& operator=(byte a[16]) {
    hiHat[0] = a[0];
    hiHat[1] = a[1];
    cymbal[0] = a[2];
    cymbal[1] = a[3];
    tomTom[0] = a[4];
    tomTom[1] = a[5];
    snare[0] = a[6];
    snare[1] = a[7];
    bassDrum[0] = a[8];
    bassDrum[1] = a[9];
    floorTom[0] = a[10];
    floorTom[1] = a[11];
    hiHatFoot[0] = a[12];
    hiHatFoot[1] = a[13];
    hiHat2[0] = a[14];
    hiHat2[1] = a[15];
    return *this;
  }
  
  // Prints current bytes on eeprom
  void printConfig();
};

// configuration_t:
// stepSequence - Holds the current configuration, read from the EEPROM.
// config_vers - Consists of an object of the stepSequencer_t, and the current configuration (DEF or NEW)
typedef struct
{
  char config_vers[CONFIG_VERSION_SIZE] = DEFAULT_CONFIG_VERSION;
  stepSequencer_t stepSequence = DEFAULT_SEQUENCE_ALL;
  
  // Print current stepSequence
  void printSequence();
  
  // convert stepSequence to bytes
  void convertToBytes(byte* inputString);

} configuration_t;


class Eeprom_t {
private:
  configuration_t currentConfig;

  // load config from EEPROM and save on currentConfig
  void loadConfig();

  // save config on currentConfig to EEPROM
  void saveConfig(char option[], unsigned int writeStart);

public:
  Eeprom_t() = default;

  // Receives an action
  // - if NEW: save new sequence
  // - if DEF: save default sequence
  void executeAction(char action[3], byte sequence[16] = NULL);
};

}

#endif