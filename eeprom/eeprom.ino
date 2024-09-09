#include <EEPROM.h>

#include "common.h"
#include "utils.h"
#include "eeprom.h"
#include "interrupt.h"

// remover esta parte ________________

#define HIHAT_DEFAULT {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
#define CYMBAL_DEFAULT {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define TOMTOM_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}
#define SNARE_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}
#define FLOORTOM_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}
#define HIHATFOOT_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}
#define BASSDRUM_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}


typedef struct{
  bool hiHat[16];
  bool cymbal[16];
  bool tomTom[16];
  bool snare[16];
  bool bassDrum[16];
  bool floorTom[16];
  bool hiHatFoot[16];
}stepSequencer_t;

stepSequencer_t stepSeq = { HIHAT_DEFAULT, CYMBAL_DEFAULT, TOMTOM_DEFAULT, SNARE_DEFAULT, FLOORTOM_DEFAULT, HIHATFOOT_DEFAULT, BASSDRUM_DEFAULT };

// remover esta parte ________________ ^^^^


interrupt::interruptCycle cycleRunner;

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

  eeprom::Eeprom_t EepromReader;
  byte seq[16] = { 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 255, 0, 0, 0, 0, 0 };

  EepromReader.executeAction("NEW", seq);
}

void loop() {
  //every 5s increment and save the settings!
  if(cycleRunner.stepCycle){
      //TODO por todos os gpios a low e atualizar com o valor correto (avaliar se precisa de um timer)
      digitalWrite(13, LOW);
      delay(50); // isto terá de ser relativo á frequência (que percentagem da duração do step fica a low)
      Serial.println("^_^");
      Serial.println(cycleRunner.stepCycle);

      Serial.println(stepSeq.hiHat[cycleRunner.stepCounter]);   
      // Serial.println(stepSeq.hiHat[cycleRunner.stepCounter]);   
      // digitalWrite(13, stepSeq.hiHat[cycleRunner.stepCounter]);
      // Serial.println(stepSeq.cymbal[cycleRunner.stepCounter]);   
      // Serial.println(stepSeq.tomTom[cycleRunner.stepCounter]);   
      // Serial.println(stepSeq.snare[cycleRunner.stepCounter]);   
      // Serial.println(stepSeq.bassDrum[cycleRunner.stepCounter]);   
      // Serial.println(stepSeq.floorTom[cycleRunner.stepCounter]);   
      // Serial.println(stepSeq.hiHatFoot[cycleRunner.stepCounter]);
      cycleRunner.stepCycle=false;
    }  //CONFIGURATION.stepSequence[0]++;
  //saveConfig();
}
