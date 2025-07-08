#include <EEPROM.h>

#include "common.h"
#include "utils.h"
#include "eeprom.h"
#include "interrupt.h"
#include "stepsequencer.h"
#include "serialcomm.h"

// Set up the needed components
stepsequencer::stepSequencer_t currentSequence;
interrupt::interruptCycle cycleRunner;
eeprom::Eeprom_t EepromReader;
serialcomm::SerialComm SerialReader;

//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
ISR(TIMER1_COMPA_vect) {
  cycleRunner.stepCounter++;
  if (cycleRunner.stepCounter == 16) cycleRunner.stepCounter = 0;
  cycleRunner.stepCycle = true;
}

void setup() {
  cycleRunner.setupInterrupt(500);

  // BAUD rate
  Serial.begin(BAUDRATE);

  uint16_t seq[CONFIG_NUM_INSTRUMENTS] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0, 0 };
  uint16_t new_seq[CONFIG_NUM_INSTRUMENTS];
  EepromReader.processSequence("NEW", seq);
  EepromReader.getSequence(new_seq);
  EepromReader.printSequence();
  currentSequence.saveSequence(new_seq);
}

void loop() {
  SerialReader.getBytesFromSerial();
  SerialReader.showNewData();
  //every 5s increment and save the settings!
  // Serial.println("^_^");
  if (cycleRunner.stepCycle) {
    // TODO: Set all GPIOs to low and update with correct value (evaluate if timer is needed)
    digitalWrite(13, LOW);
    delay(50); // This should be relative to frequency (what % of step duration stays low)
    Serial.print(cycleRunner.stepCounter);
    Serial.print("  ");
    Serial.print(currentSequence.hiHat[cycleRunner.stepCounter]);
    Serial.print("  ");
    Serial.print(currentSequence.cymbal[cycleRunner.stepCounter]);
    Serial.print("  ");
    Serial.print(currentSequence.tomTom[cycleRunner.stepCounter]);
    Serial.print("  ");
    Serial.print(currentSequence.snare[cycleRunner.stepCounter]);
    Serial.print("  ");
    Serial.print(currentSequence.bassDrum[cycleRunner.stepCounter]);
    Serial.print("  ");
    Serial.print(currentSequence.floorTom[cycleRunner.stepCounter]);
    Serial.print("  ");
    Serial.print(currentSequence.hiHatFoot[cycleRunner.stepCounter]);
    Serial.print("  ");
    Serial.print(currentSequence.hiHat2[cycleRunner.stepCounter]);
    Serial.println("  ");
    digitalWrite(13, currentSequence.hiHat[cycleRunner.stepCounter]);
    cycleRunner.stepCycle = false;
  }
  // CONFIGURATION.stepSequence[0]++;
  // saveConfig();
}
