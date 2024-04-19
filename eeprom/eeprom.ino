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

void printBin(byte aByte) {
  for (int8_t aBit = 7; aBit >= 0; aBit--)
    Serial.write(bitRead(aByte, aBit) ? '1' : '0');
}
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

  printConfig(){
    Serial.println("///////////// PRINTING CONFIG \\\\\\\\\\\\\\\\\\");
    Serial.print("hiHat:     ");
    printBin(hiHat[0]);
    printBin(hiHat[1]);
    Serial.println("");
    Serial.print("cymbal:    ");
    printBin(cymbal[0]);
    printBin(cymbal[1]);
    Serial.println("");
    Serial.print("tomTom:    ");
    printBin(tomTom[0]);
    printBin(tomTom[1]);
    Serial.println("");
    Serial.print("snare:     ");
    printBin(snare[0]);
    printBin(snare[1]);
    Serial.println("");
    Serial.print("bassDrum:  ");
    printBin(bassDrum[0]);
    printBin(bassDrum[1]);
    Serial.println("");
    Serial.print("floorTom:  ");
    printBin(floorTom[0]);
    printBin(floorTom[1]);
    Serial.println("");
    Serial.print("hiHatFoot: ");
    printBin(hiHatFoot[0]);
    printBin(hiHatFoot[1]);
    Serial.println("");
    Serial.print("hiHat2:    ");
    printBin(hiHat2[0]);
    printBin(hiHat2[1]);
    Serial.println("");
  }
};

typedef struct
{
  char config_vers[CONFIG_VERSION_SIZE] = DEFAULT_CONFIG_VERSION;
  stepSequencer_t stepSequence = DEFAULT_SEQUENCE_ALL;

  void printSequence() {
    stepSequence.printConfig();
  }
  void convertToBytes(byte* a){
    a[0] = stepSequence.hiHat[0];
    a[1] = stepSequence.hiHat[1];
    a[2] = stepSequence.cymbal[0];
    a[3] = stepSequence.cymbal[1];
    a[4] = stepSequence.tomTom[0];
    a[5] = stepSequence.tomTom[1];
    a[6] = stepSequence.snare[0];
    a[7] = stepSequence.snare[1];
    a[8] = stepSequence.bassDrum[0];
    a[9] = stepSequence.bassDrum[1];
    a[10] = stepSequence.floorTom[0];
    a[11] = stepSequence.floorTom[1];
    a[12] = stepSequence.hiHatFoot[0];
    a[13] = stepSequence.hiHatFoot[1];
    a[14] = stepSequence.hiHat2[0];
    a[15] = stepSequence.hiHat2[1];
  }
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
    byte sequenceFromEeprom[16];
    // lê a versão
    readEeprom(CONFIG_START, CONFIG_VERSION_SIZE, currentConfig.config_vers);
    // lê a sequência
    readEeprom(CONFIG_START+CONFIG_VERSION_SIZE, 16, sequenceFromEeprom);
    currentConfig.stepSequence = sequenceFromEeprom;

    Serial.println(currentConfig.config_vers);
    currentConfig.printSequence();
  }

  // save config só vai buscar ao que está guardado na struct e envia para o eeprom, mudar. depois replicar o que pensei
  // para o load
  void saveConfig(char option[], unsigned int writeStart) {
    Serial.println("saveConfig");
    byte sequenceToEeprom[16];

    currentConfig.convertToBytes(sequenceToEeprom);
    Serial.println(sequenceToEeprom[15],BIN);
    Serial.println(sequenceToEeprom[14],BIN);
    Serial.println(sequenceToEeprom[13],BIN);

    saveStringOnEeprom(option, writeStart);
    saveEeprom(sequenceToEeprom, 16, writeStart + CONFIG_VERSION_SIZE);
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
    currentConfig.printSequence();
    
    return;
  }
};


void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");  // prints hello with ending line break

  Eeprom_t EepromReader;
  byte seq[16] = { 0, 0, 0, 0, 0, 0, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  EepromReader.executeAction("NEW", seq);
}

void loop() {
  //every 5s increment and save the settings!
  delay(500);
  //CONFIGURATION.stepSequence[0]++;
  //saveConfig();
}
