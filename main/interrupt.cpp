#include "interrupt.h"
namespace interrupt {

uint16_t msToOcr1a(const int &milliseconds){
  return MSTOOCR1A_FACTOR*milliseconds-1;
}


void interruptCycle::setupInterrupt(const int &frequencyMs){

  //stop interrupts
  cli();

  // set entire TCCR1A register to 0
  TCCR1A = 0;
  // same for TCCR1B
  TCCR1B = 0;
  //initialize counter value to 0
  TCNT1  = 0;
  // set compare match register for 1hz increments
  OCR1A = msToOcr1a(frequencyMs); // set the milliseconds for the timer

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  //allow interrupts
  sei();
}


}


