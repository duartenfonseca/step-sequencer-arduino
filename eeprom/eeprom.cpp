#include "eeprom.h"

namespace eeprom {

  void readFromEeprom(int readStart, int readSize, char* outputBuffer) {
    for (int i = 0; i < readSize; i++) {
      outputBuffer[i] = EEPROM.read(readStart + i);
    }
  }

  void readFromEeprom(int readStart, int readSize, uint16_t* outputBuffer) {
    // EEPROM (char)           0xff 0xff
    // outputBuffer (uint16_t) 0xffff
    for (int i = 0; i < readSize; i++) {
      outputBuffer[i] = (EEPROM.read(readStart + i * 2) << 8) + (EEPROM.read(readStart + i * 2 + 1));
    }
  }

  void saveByteOnEeprom(byte l_byte, unsigned int readPosition) {
    byte l_read = EEPROM.read(readPosition);

    if (l_read != l_byte) {
      EEPROM.write(readPosition, l_byte);
    }
  }

  void saveArrayOnEeprom(uint16_t* string, size_t arraySize, unsigned int writeStart) {
    for (size_t i = 0; i < arraySize; i++) {
      saveByteOnEeprom((string[i] >> 8), writeStart + 2 * i);
      saveByteOnEeprom((string[i] & 0xff), writeStart + 2 * i + 1);
    }
  }

  void saveArrayOnEeprom(byte* string, size_t arraySize, unsigned int writeStart) {
    for (size_t i = 0; i < arraySize; i++) {
      saveByteOnEeprom(string[i], writeStart + i);
    }

    // write the NULL termination
    if (EEPROM.read(writeStart + arraySize) != 0)
      EEPROM.write(writeStart + arraySize, 0);
  }

  void printEeprom(int numberOfBytes) {
    byte stringToPrint[numberOfBytes];

    Serial.println("///////////// PRINTING EEPROM \\\\\\\\\\\\\\\\\\");
    readFromEeprom(CONFIG_START, numberOfBytes, stringToPrint);
    for (int i = 0; i < numberOfBytes; i++) {
      ((i < 3) ? Serial.println((char)stringToPrint[i]) : Serial.println(stringToPrint[i], HEX));
    }
  }

  void Eeprom_t::loadConfig() {
    // read version
    readFromEeprom(CONFIG_START, CONFIG_ACTION_SIZE, savedAction);

    // read sequence
    readFromEeprom(CONFIG_START + CONFIG_ACTION_SIZE, CONFIG_NUM_INSTRUMENTS, savedSequence);
  }

  void Eeprom_t::saveConfig(char action[], unsigned int writeStart) {
    saveArrayOnEeprom(action, CONFIG_ACTION_SIZE - 1, writeStart);
    saveArrayOnEeprom(savedSequence, BYTE_SIZE, writeStart + CONFIG_ACTION_SIZE);
  }

  void Eeprom_t::processSequence(char newAction[CONFIG_ACTION_SIZE], uint16_t newSequence[BYTE_SIZE] = NULL) {
    // Load saved config
    loadConfig();
    uint16_t defaultSequence[CONFIG_NUM_INSTRUMENTS] = DEFAULT_SEQUENCE_ALL;

    // if action is default, save default configuration
    if ((strcmp(newAction, DEFAULT_CONFIG_ACTION) == 0)) {
      if (strcmp(savedAction, newAction) == 0) {
        return;
      }
      if ((strlcpy(savedAction, newAction, CONFIG_ACTION_SIZE)) >= CONFIG_ACTION_SIZE) {
        Serial.println("Error reading saving config version: Action string too big");
      }
      for (int i = 0; i < CONFIG_NUM_INSTRUMENTS; i++) {
        savedSequence[i] = defaultSequence[i];
      }
    }

    // if action is NEW, save the new sequence on the EEPROM
    else if (strcmp(newAction, NEW_CONFIG_ACTION) == 0) {
      if ((strlcpy(savedAction, newAction, CONFIG_ACTION_SIZE)) >= CONFIG_ACTION_SIZE) {
        Serial.println("Error reading saving config version: Action string too big");
      }

      for (int i = 0; i < CONFIG_NUM_INSTRUMENTS; i++) {
        savedSequence[i] = newSequence[i];
      }
    }
    printEeprom(20);
    saveConfig(newAction, CONFIG_START);
    return;
  }

  void Eeprom_t::getSequence(uint16_t* sequence) {
    for (int i = 0; i < CONFIG_NUM_INSTRUMENTS; i++) {
      sequence[i] = savedSequence[i];
    }
  }

  void Eeprom_t::printSequence() {
    Serial.println("///////////// PRINTING SAVED SEQUENCE \\\\\\\\\\\\\\\\\\");
    for (int i = 0; i < CONFIG_NUM_INSTRUMENTS; i++) {
      utils::printSequence(savedSequence[i]);
    }
  }
}