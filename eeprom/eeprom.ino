#include <EEPROM.h>

#define DEFAULT_CONFIG_VERSION "DEF"
#define NEW_CONFIG_VERSION "NEW"
#define CONFIG_VERSION_SIZE 4  // plus one for null termination

// EEPROM MEMORY starting position
#define CONFIG_START 32
#define DEFAULT_SEQUENCE 255
#define DEFAULT_SEQUENCE_ALL \
  { \
    DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, \
      DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, \
      DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, \
      DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE, DEFAULT_SEQUENCE \
  }

// templated sizeof function for arrays
template<size_t N>
struct type_of_size {
  typedef char type[N];
};

template<typename T, size_t Size>
typename type_of_size<Size>::type& sizeof_array_helper(T (&)[Size]);
#define sizeof_array(pArray) sizeof(sizeof_array_helper(pArray))

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
};

typedef struct
{
  char config_vers[CONFIG_VERSION_SIZE] = DEFAULT_CONFIG_VERSION;
  stepSequencer_t stepSequence = DEFAULT_SEQUENCE_ALL;
} configuration_type;


void printEeprom(int numberOfBytes) {
  Serial.println("///////////// PRINTING EEPROM \\\\\\\\\\\\\\\\\\");
  byte test[numberOfBytes];
  readEeprom(CONFIG_START, numberOfBytes, test);
  for (int i = 0; i < numberOfBytes; i++) {
    ((i < 3) ? Serial.println((char)test[i]) : Serial.println(test[i], BIN));
  }
}

// save bytes into eeprom
void saveEeprom(byte string[], size_t arraySize, unsigned int writeStart) {
  for (size_t i = 0; i < arraySize; i++) {
    byte l_byte = string[i];
    byte l_read = EEPROM.read(writeStart + i);

    if (l_read != l_byte) {
      EEPROM.write(writeStart + i, l_byte);
    }
  }
}

// save strings (w null termination) to eeprom
void saveStringOnEeprom(byte string[], unsigned int writeStart) {
  saveEeprom(string, strlen(string), writeStart);

  //write the NULL termination
  if (EEPROM.read(writeStart + strlen(string)) != 0)
    EEPROM.write(writeStart + strlen(string), 0);
}
// Reads saved eeprom values and saves them on outputBuffer 
void readEeprom(int readStart, int readSize, char* outputBuffer) {
  for (int i = 0; i < readSize; i++) {
    outputBuffer[i] = EEPROM.read(readStart + i);
  }
}

void convertSequenceToBytes(int sequenceArray[], size_t sizeOfsequenceArray, byte byteArray[]) {
  for (int i = 0; i < sizeOfsequenceArray; i++) {
    bitWrite(byteArray[0], i, sequenceArray[sizeOfsequenceArray - i - 1]);
    bitWrite(byteArray[1], i, sequenceArray[sizeOfsequenceArray - i - 1 - 8]);
  }
}

class Eeprom_t {
private:
  configuration_type currentConfig;

  // TODO: falta adicionar o resto do load, da parte da sequência. trocar para guardar a seq num array
  void loadConfig() {
    readEeprom(CONFIG_START, CONFIG_VERSION_SIZE, currentConfig.config_vers);
    // readEeprom(CONFIG_START+CONFIG_VERSION_SIZE, CONFIG_START+CONFIG_VERSION_SIZE + 1, currentConfig.stepSequence[0]);
    Serial.println(currentConfig.config_vers);
    // Serial.println(currentConfig.stepSequence[0]);
  }

  // save config só vai buscar ao que está guardado na struct e envia para o eeprom, mudar. depois replicar o que pensei
  // para o load
  void saveConfig(char option[], unsigned int writeStart) {
    byte byteArray[2];

    // convertSequenceToBytes(currentConfig.stepSequence,sizeof_array(currentConfig.stepSequence),byteArray);
    saveStringOnEeprom(option, writeStart);
    saveEeprom(byteArray, sizeof_array(byteArray), writeStart + CONFIG_VERSION_SIZE);
    saveEeprom(byteArray, sizeof_array(byteArray), writeStart + CONFIG_VERSION_SIZE + sizeof_array(byteArray));
  }

public:
  Eeprom_t() = default;

  // quando recebe uma ação ( envio de alteração por COM):
  // lê a config, vê se precisa de atualizar a config e grava-a se necessário
  void executeAction(char action[3], byte sequence[16] = NULL) {
    // Read saved version config
    loadConfig();

    // se receber DEF faz reset á sequência ou mantém igual
    if ((strcmp(action, DEFAULT_CONFIG_VERSION) == 0)) {
      // se a ação for default, e a config já estiver default, não fazer nada
      if (strcmp(currentConfig.config_vers, action) == 0) {
        return;
      }
      strcpy(currentConfig.config_vers, action);
      currentConfig.stepSequence = DEFAULT_SEQUENCE_ALL;
    }

    // se receber NEW muda para a sequência nova
    else if (strcmp(action, NEW_CONFIG_VERSION) == 0) {
      strcpy(currentConfig.config_vers, action);
      currentConfig.stepSequence = sequence;
    }

    saveConfig(action,CONFIG_START);

    // se nenhuma config foi pedida dar erro?
    return;
  }

};


void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");  // prints hello with ending line break

  Eeprom_t EepromReader;
  byte seq[16] = { 1, 1, 255, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

  EepromReader.executeAction("ASD", seq);
}

void loop() {
  //every 5s increment and save the settings!
  delay(500);
  //CONFIGURATION.stepSequence[0]++;
  //saveConfig();
}
