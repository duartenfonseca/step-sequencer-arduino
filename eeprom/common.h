/*
  common.h - Configurations of the project

*/
#ifndef Common_h
#define Common_h

////////////////////////////////    EEPROM   \\\\\\\\\\\\\\\\\\\\\\\\\\

// Config definitions
#define DEFAULT_CONFIG_VERSION "DEF"
#define NEW_CONFIG_VERSION "NEW"
#define CONFIG_VERSION_SIZE 4  // three plus one for null termination
#define CONFIG_SEQUENCE_SIZE 8

// EEPROM memory starting position
#define CONFIG_START 32

////////////////////////////////    SEQUENCE   \\\\\\\\\\\\\\\\\\\\\\\\\

#define DEFAULT_SEQUENCE 0
#define DEFAULT_SEQUENCE_ALL \
  { \
    DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, \
    DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE \
  }

///////////////////////////////    INTERRUPT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#define MSTOOCR1A_FACTOR (16000000.0)/(2000.0*1024.0)


#endif