//this code will enable all three arduino timer interrupts.
//timer1 will interrupt at 1Hz

//storage variables
bool toggle0 = 0;
bool toggle1 = 0;
bool toggle2 = 0;
long int t2 = 0;
long int t1 = 0;

// Get the OCR1A value from the number of milliseconds for the loop
long msToOcr1a(int milliseconds){
  long result;
  result = (16*pow(10,6)*milliseconds)/((long)2000*1024)-1;
  return result;
}

void setup() {

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);

  // Baud Rate
  Serial.begin(115200);

  // Auxiliar variable to check timing of interrupt
  long int t1 = millis();

  cli();  //stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = msToOcr1a(10); // set the milliseconds for the timer
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();  //allow interrupts

}  //end setup

ISR(TIMER1_COMPA_vect) {  //timer1 interrupt 1Hz toggles pin 13 (LED)
                          //generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle1) {
    digitalWrite(13, HIGH);
    t2 = millis();
    Serial.print("high: ");
    Serial.print(t2 - t1);
    t1 = t2;
    Serial.println(" milliseconds");
    toggle1 = 0;
  }

  else {
    digitalWrite(13, LOW);
    toggle1 = 1;
  }
}

void loop() {
}