/*
  common.h - Configurations of the project

*/
#ifndef Common_h
#define Common_h

////////////////////////////////    EEPROM   \\\\\\\\\\\\\\\\\\\\\\\\\\

#define BAUDRATE 9600
#define DEFAULT_CONFIG_ACTION "DEF"
#define NEW_CONFIG_ACTION "NEW"
#define CONFIG_ACTION_SIZE 4  // three plus one for null termination
#define CONFIG_NUM_INSTRUMENTS 8
#define BYTE_SIZE 8
#define TWO_BYTE_SIZE 16
#define CONFIG_START 32   // Start of configuration in EEPROM

////////////////////////////////    SEQUENCE   \\\\\\\\\\\\\\\\\\\\\\\\\

#define DEFAULT_SEQUENCE 0
#define DEFAULT_SEQUENCE_ALL \
  { \
    DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, \
    DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE \
  }
#define NUM_STEPS 16

///////////////////////////////    INTERRUPT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#define MSTOOCR1A_FACTOR (16000000.0)/(2000.0*1024.0)

//////////////////////////////    SERIAL COMM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#define SERIAL_NUM_BYTES 8

#endif