#include "eeprom.h"

namespace eeprom {

  void readCharFromEeprom(int readStart, int readSize, char* outputBuffer) {
    for (int i = 0; i < readSize; i++) {
      outputBuffer[i] = EEPROM.read(readStart + i);
    }
  }

  void read16BitFromEeprom(int readStart, int readSize, uint16_t* outputBuffer) {
    // EEPROM (char)           0xff 0xff
    // outputBuffer (uint16_t) 0xffff
    for (int i = 0; i < readSize/2; i++) {
      outputBuffer[i] = (EEPROM.read(readStart + i*2)<<8)+(EEPROM.read(readStart + i*2 +1 ));
    }
  }

  void saveByteOnEeprom(byte l_byte, unsigned int readPosition) {
      byte l_read = EEPROM.read(readPosition);

      if (l_read != l_byte) {
        EEPROM.write(readPosition, l_byte);
      }
  }
  void saveArrayOnEeprom(byte* string, size_t arraySize, unsigned int writeStart){
    for (size_t i = 0; i < arraySize; i++) {
      saveByteOnEeprom(string[i],writeStart+i);
    }
  }
  void save16BitArrayOnEeprom(uint16_t* string, size_t arraySize, unsigned int writeStart){
    for (size_t i = 0; i < arraySize; i++) {
      saveByteOnEeprom((string[i] >> 8),writeStart +2*i);
      saveByteOnEeprom((string[i] & 0xff),writeStart +2*i +1);
    }
  }
  void saveStringOnEeprom(byte* string, size_t arraySize, unsigned int writeStart) {
    saveArrayOnEeprom(string, arraySize, writeStart);

    //write the NULL termination
    if (EEPROM.read(writeStart + arraySize) != 0)
      EEPROM.write(writeStart + arraySize, 0);
  }

  void printEeprom(int numberOfBytes) {
    byte stringToPrint[numberOfBytes];
    
    Serial.println("///////////// PRINTING EEPROM \\\\\\\\\\\\\\\\\\");
    readCharFromEeprom(CONFIG_START, numberOfBytes, stringToPrint);
    for (int i = 0; i < numberOfBytes; i++) {
      ((i < 3) ? Serial.println((char)stringToPrint[i]) : Serial.println(stringToPrint[i], HEX));
    }
  }
  
  // void configuration_t::convertToBytes(byte* inputString){
  //   inputString[0] = stepSequence.hiHat[0];
  //   inputString[1] = stepSequence.hiHat[1];
  //   inputString[2] = stepSequence.cymbal[0];
  //   inputString[3] = stepSequence.cymbal[1];
  //   inputString[4] = stepSequence.tomTom[0];
  //   inputString[5] = stepSequence.tomTom[1];
  //   inputString[6] = stepSequence.snare[0];
  //   inputString[7] = stepSequence.snare[1];
  //   inputString[8] = stepSequence.bassDrum[0];
  //   inputString[9] = stepSequence.bassDrum[1];
  //   inputString[10] = stepSequence.floorTom[0];
  //   inputString[11] = stepSequence.floorTom[1];
  //   inputString[12] = stepSequence.hiHatFoot[0];
  //   inputString[13] = stepSequence.hiHatFoot[1];
  //   inputString[14] = stepSequence.hiHat2[0];
  //   inputString[15] = stepSequence.hiHat2[1];
  // }

  void Eeprom_t::loadConfig() {
    uint16_t sequenceFromEeprom[8];

    // read version
    readCharFromEeprom(CONFIG_START, CONFIG_VERSION_SIZE, config_vers);

    // read sequence
    read16BitFromEeprom(CONFIG_START+CONFIG_VERSION_SIZE, 16, saved_sequence);

    printEeprom(20);
  }

  void Eeprom_t::saveConfig(char option[], unsigned int writeStart) {
    saveStringOnEeprom(option,strlen(option), writeStart);
    save16BitArrayOnEeprom(saved_sequence, 8, writeStart + CONFIG_VERSION_SIZE);
  }

  void Eeprom_t::processSequence(char action[3], uint16_t new_sequence[8] = NULL) {
    // Load saved config
    loadConfig();
    uint16_t default_sequence[CONFIG_SEQUENCE_SIZE] = DEFAULT_SEQUENCE_ALL;

    // if action is default, save default configuration
    if ((strcmp(action, DEFAULT_CONFIG_VERSION) == 0)) {
      if (strcmp(config_vers, action) == 0) {
        return;
      }
      if ((strlcpy(config_vers, action, CONFIG_VERSION_SIZE))>= CONFIG_VERSION_SIZE){
        Serial.println("Error reading saving config version: Action string too big");
      }
      for (int i = 0; i < CONFIG_SEQUENCE_SIZE; i++) {
        saved_sequence[i] = default_sequence[i];
      }
    }

    // if action is NEW, save the new sequence on the EEPROM
    else if (strcmp(action, NEW_CONFIG_VERSION) == 0) {
      if((strlcpy(config_vers, action, CONFIG_VERSION_SIZE))>= CONFIG_VERSION_SIZE){
        Serial.println("Error reading saving config version: Action string too big");
      }
      
      for (int i = 0; i < 8; i++) {
        saved_sequence[i] = new_sequence[i];
      }
    }
    printEeprom(20);
    saveConfig(action,CONFIG_START);
    return;
  }

  void Eeprom_t::getSequence(uint16_t *sequence){
    for (int i = 0; i < CONFIG_SEQUENCE_SIZE; i++) {
      sequence[i] = saved_sequence[i];
    }
  }

  void Eeprom_t::printSequence(){
    for (int i = 0; i < CONFIG_SEQUENCE_SIZE; i++) {
      utils::printSequence(saved_sequence[i]);
    }
  }
}