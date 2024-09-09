#include "eeprom.h"

namespace eeprom {

  void readEeprom(int readStart, int readSize, char* outputBuffer) {
    for (int i = 0; i < readSize; i++) {
      outputBuffer[i] = EEPROM.read(readStart + i);
    }
  }

  void saveEeprom(byte* string, size_t arraySize, unsigned int writeStart) {
    for (size_t i = 0; i < arraySize; i++) {
      byte l_byte = string[i];
      byte l_read = EEPROM.read(writeStart + i);

      if (l_read != l_byte) {
        EEPROM.write(writeStart + i, l_byte);
      }
    }
  }

  void saveStringOnEeprom(byte* string, size_t arraySize, unsigned int writeStart) {
    saveEeprom(string, arraySize, writeStart);

    //write the NULL termination
    if (EEPROM.read(writeStart + arraySize) != 0)
      EEPROM.write(writeStart + arraySize, 0);
  }

  void printEeprom(int numberOfBytes) {
    byte stringToPrint[numberOfBytes];
    
    Serial.println("///////////// PRINTING EEPROM \\\\\\\\\\\\\\\\\\");
    readEeprom(CONFIG_START, numberOfBytes, stringToPrint);
    for (int i = 0; i < numberOfBytes; i++) {
      ((i < 3) ? Serial.println((char)stringToPrint[i]) : Serial.println(stringToPrint[i], BIN));
    }
  }
  
  void stepSequencer_t::printConfig(){
    Serial.println("///////////// PRINTING CONFIG \\\\\\\\\\\\\\\\\\");
    utils::printSequence("hiHat", hiHat);
    utils::printSequence("cymbal", cymbal);
    utils::printSequence("tomTom", tomTom);
    utils::printSequence("snare", snare);
    utils::printSequence("bassDrum", bassDrum);
    utils::printSequence("floorTom", floorTom);
    utils::printSequence("hiHatFoot", hiHatFoot);
    utils::printSequence("hiHat2", hiHat2);
  }

  void configuration_t::printSequence() {
    stepSequence.printConfig();
  }

  void configuration_t::convertToBytes(byte* inputString){
    inputString[0] = stepSequence.hiHat[0];
    inputString[1] = stepSequence.hiHat[1];
    inputString[2] = stepSequence.cymbal[0];
    inputString[3] = stepSequence.cymbal[1];
    inputString[4] = stepSequence.tomTom[0];
    inputString[5] = stepSequence.tomTom[1];
    inputString[6] = stepSequence.snare[0];
    inputString[7] = stepSequence.snare[1];
    inputString[8] = stepSequence.bassDrum[0];
    inputString[9] = stepSequence.bassDrum[1];
    inputString[10] = stepSequence.floorTom[0];
    inputString[11] = stepSequence.floorTom[1];
    inputString[12] = stepSequence.hiHatFoot[0];
    inputString[13] = stepSequence.hiHatFoot[1];
    inputString[14] = stepSequence.hiHat2[0];
    inputString[15] = stepSequence.hiHat2[1];
  }

  void Eeprom_t::loadConfig() {
    byte sequenceFromEeprom[16];
    
    // read version
    readEeprom(CONFIG_START, CONFIG_VERSION_SIZE, currentConfig.config_vers);
    
    // read sequence
    readEeprom(CONFIG_START+CONFIG_VERSION_SIZE, 16, sequenceFromEeprom);
    currentConfig.stepSequence = sequenceFromEeprom;

    currentConfig.printSequence();
  }

  void Eeprom_t::saveConfig(char option[], unsigned int writeStart) {
    byte sequenceToEeprom[16];

    currentConfig.convertToBytes(sequenceToEeprom);

    saveStringOnEeprom(option,strlen(option), writeStart);
    saveEeprom(sequenceToEeprom, 16, writeStart + CONFIG_VERSION_SIZE);
  }

  void Eeprom_t::executeAction(char action[3], byte sequence[16] = NULL) {
    // Read saved version config
    loadConfig();

    // if action is default, save default configuration
    if ((strcmp(action, DEFAULT_CONFIG_VERSION) == 0)) {

      if (strcmp(currentConfig.config_vers, action) == 0) {
        return;
      }
      strcpy(currentConfig.config_vers, action);
      currentConfig.stepSequence = DEFAULT_SEQUENCE_ALL;
    }

    // if action is NEW, save the new sequence on the EEPROM
    else if (strcmp(action, NEW_CONFIG_VERSION) == 0) {
      strcpy(currentConfig.config_vers, action);
      currentConfig.stepSequence = sequence;
    }

    saveConfig(action,CONFIG_START);
    currentConfig.printSequence();
    
    return;
  }
}