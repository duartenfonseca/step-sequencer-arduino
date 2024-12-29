#include <EEPROM.h>

#include "common.h"
#include "utils.h"
#include "eeprom.h"
#include "interrupt.h"
#include "stepsequencer.h"

// Set up the needed components
stepsequencer::stepSequencer_t currentSequence;
interrupt::interruptCycle cycleRunner;
eeprom::Eeprom_t EepromReader;

//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
ISR(TIMER1_COMPA_vect) {  
                        
  cycleRunner.stepCounter++;
  if (cycleRunner.stepCounter == 16) cycleRunner.stepCounter = 0;
  cycleRunner.stepCycle = true;
};


void setup() {
  cycleRunner.setupInterrupt(500);

  // BAUD rate
  Serial.begin(9600);

  
  uint16_t seq[8] = { 0xFFFF, 0X1111, 0x2220, 0xDDDD, 0, 0, 0, 0};
  uint16_t new_seq[8];
  EepromReader.processSequence("NEW", seq);
  EepromReader.getSequence(new_seq);
  EepromReader.printSequence();
  currentSequence.saveSequence(new_seq);
}

void loop() {
  //every 5s increment and save the settings!
  if(cycleRunner.stepCycle){
      //TODO por todos os gpios a low e atualizar com o valor correto (avaliar se precisa de um timer)
      // digitalWrite(13, LOW);
      // delay(50); // isto terá de ser relativo á frequência (que percentagem da duração do step fica a low)
      // Serial.println("^_^");
      // Serial.println(cycleRunner.stepCycle);
      // Serial.println(currentSequence.hiHat[cycleRunner.stepCounter]);
      // digitalWrite(13, currentSequence.hiHat[cycleRunner.stepCounter]);
      // Serial.println(currentSequence.cymbal[cycleRunner.stepCounter]);
      // Serial.println(currentSequence.tomTom[cycleRunner.stepCounter]);
      // Serial.println(currentSequence.snare[cycleRunner.stepCounter]);
      // Serial.println(currentSequence.bassDrum[cycleRunner.stepCounter]);
      // Serial.println(currentSequence.floorTom[cycleRunner.stepCounter]);
      // Serial.println(currentSequence.hiHatFoot[cycleRunner.stepCounter]);
      cycleRunner.stepCycle=false;
    }  //CONFIGURATION.stepSequence[0]++;
  //saveConfig();
}
