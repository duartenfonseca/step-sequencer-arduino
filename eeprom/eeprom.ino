#include <EEPROM.h>
 
#define DEFAULT_CONFIG_VERSION "DEF"
#define NEW_CONFIG_VERSION "NEW"
#define CONFIG_VERSION_SIZE 4 // plus one for null termination
#define ARRAYSIZE(x)  (sizeof(x) / sizeof(x[0]))

// Where in EEPROM?
#define CONFIG_START 32
#define DEFAULT_SEQUENCE \
 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }


// templated sizeof function for arrays
template <size_t N>
struct type_of_size
{
  typedef char type[N];
};

template <typename T, size_t Size>
typename type_of_size<Size>::type& sizeof_array_helper(T(&)[Size]);
#define sizeof_array(pArray) sizeof(sizeof_array_helper(pArray))

void printEeprom(int numberOfBytes){
  Serial.println("///////////// PRINTING EEPROM \\\\\\\\\\\\\\\\\\");
  byte test[numberOfBytes];
  readEeprom(CONFIG_START, numberOfBytes, test);
  for (int i= 0; i<numberOfBytes; i++){
    ((i < 3) ? Serial.println((char)test[i]) :Serial.println(test[i],BIN));
  }
}

void saveEeprom(byte string[],size_t arraySize, unsigned int writeStart) {
  for (size_t i = 0; i < arraySize; i ++) {
    byte l_byte = string[i];
    byte l_read = EEPROM.read(writeStart + i);

    if (l_read != l_byte) {
      EEPROM.write(writeStart + i, l_byte);
    }
  }
}

// save the CONFIGURATION in to EEPROM
void saveStringOnEeprom(byte string[], unsigned int writeStart) {
  saveEeprom(string,strlen(string),writeStart);

  //write the NULL termination
  if (EEPROM.read(writeStart + strlen(string)) != 0)
	  EEPROM.write(writeStart + strlen(string), 0);
}


void readEeprom(int readStart, int readSize, char* outputBuffer){
  for (int i=0; i<readSize; i++){
    outputBuffer[i] = EEPROM.read(readStart + i);
  }
}


void convertSequenceToBytes(int inputArray[],size_t sizeOfInputArray,byte byteArray[])
{
  for (int i = 0; i< sizeOfInputArray;i++)
  {
    bitWrite(byteArray[0], i , inputArray[sizeOfInputArray-i-1]);
    bitWrite(byteArray[1], i , inputArray[sizeOfInputArray-i-1-8]);
  }
}
typedef struct{
  bool hiHat[16];
  bool cymbal[16];
  bool tomTom[16];
  bool snare[16];
  bool bassDrum[16];
  bool floorTom[16];
  bool hiHatFoot[16];
}stepSequencer_t;

typedef struct
{
  char config_vers[CONFIG_VERSION_SIZE] = DEFAULT_CONFIG_VERSION;
  stepSequencer_t stepSequence = {DEFAULT_SEQUENCE};
} configuration_type;

class Eeprom_t {
private:
  configuration_type currentConfig;

  // TODO: falta adicionar o resto do load, da parte da sequência
  void loadConfig(){
    readEeprom(CONFIG_START, CONFIG_VERSION_SIZE, currentConfig.config_vers);
    printEeprom(3);

  }
  void saveConfig(char option[] , unsigned int writeStart){
    byte byteArray[2];
    
    // convertSequenceToBytes(currentConfig.stepSequence,sizeof_array(currentConfig.stepSequence),byteArray);

    saveStringOnEeprom(option,writeStart);
    saveEeprom(byteArray,sizeof_array(byteArray),writeStart+CONFIG_VERSION_SIZE);
    saveEeprom(byteArray,sizeof_array(byteArray),writeStart+CONFIG_VERSION_SIZE+sizeof_array(byteArray));

  }

public:
  Eeprom_t ()=default;

  // quando recebe uma ação ( envio de alteração por COM)
  void executeAction(char action[3]){
    // Read saved version config
    loadConfig();

    // se receber um NEW ou DEF executa a ação respetiva
    if((strcmp(action, DEFAULT_CONFIG_VERSION) == 0 ) || (strcmp(action, NEW_CONFIG_VERSION) == 0) )
      {
        // se a ação for default, e a config já estiver default, não fazer nada
        if (strcmp(currentConfig.config_vers, action) == 0){
          return;
        }
        saveConfig(action,CONFIG_START);
    }
    // se nenhuma config foi pedida dar erro?

  }


};
/*
 
// load whats in EEPROM in to the local CONFIGURATION if it is a valid setting
int loadConfig() {
  // is it correct?
  if ((EEPROM.read(CONFIG_START + 0)  == CONFIG_VERSION[0] &&
      (EEPROM.read(CONFIG_START + 0)  == CONFIG_VERSION[1] &&
      (EEPROM.read(CONFIG_START + 0)  == CONFIG_VERSION[2]){
 
  // load (overwrite) the local configuration struct
    for (unsigned int i=0; i<sizeof(CONFIGURATION); i++){
      *((char*)&CONFIGURATION + i) = EEPROM.read(CONFIG_START + i);
    }
    return 1; // return 1 if config loaded 
  }
  return 0; // return 0 if config NOT loaded
}
 */

 
void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");  // prints hello with ending line break 
 /*
  if(loadConfig()){
    Serial.println("Config loaded:");
    Serial.println(CONFIGURATION.version);
    Serial.println(CONFIGURATION.stepSequence[0]);
  }else{
    Serial.println("Config not loaded!");
    saveConfig(); // overwrite with the default settings
  }*/
  
  Eeprom_t EepromReader;
  EepromReader.executeAction("");
  



}
 
void loop() {
  //every 5s increment and save the settings!
  delay(500);
  //CONFIGURATION.stepSequence[0]++;
  //saveConfig();
}