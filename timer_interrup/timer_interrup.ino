#define HIHAT_DEFAULT \
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
#define CYMBAL_DEFAULT \
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#define TOMTOM_DEFAULT \
  { 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1 }
#define SNARE_DEFAULT \
  { 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1 }
#define FLOORTOM_DEFAULT \
  { 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1 }
#define HIHATFOOT_DEFAULT \
  { 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1 }
#define BASSDRUM_DEFAULT \
  { 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1 }

#define MSTOCR1A_FACTOR (16000000.0) / (2000.0 * 1024.0)

// storage variables
bool toggle1 = 0;
byte stepCounter = 0;
bool stepChanged = false;

// serial send bytes configuration
const byte numBytes = 5;
byte receivedBytes[numBytes];
byte numReceived = 0;
bool newData = false;
bool runSequencer = false;

// step sequencer data structure
typedef struct {
  bool hiHat[16];
  bool cymbal[16];
  bool tomTom[16];
  bool snare[16];
  bool bassDrum[16];
  bool floorTom[16];
  bool hiHatFoot[16];
  byte incomingArray[16];
} stepSequencer_t;

stepSequencer_t stepSeq = { HIHAT_DEFAULT, CYMBAL_DEFAULT, TOMTOM_DEFAULT, SNARE_DEFAULT, FLOORTOM_DEFAULT, HIHATFOOT_DEFAULT, BASSDRUM_DEFAULT };

// Receive a new sequence
void recvSequence() {
  static boolean recvInProgress = false;
  static byte pos = 0;
  byte startMarker = 0x3C;
  byte endMarker = 0x3E;
  byte receivedByte;
Serial.write(("here"));
  while (Serial.available() > 0 && newData == false) {
    receivedByte = Serial.read();

    if (recvInProgress == true) {
      if (receivedByte != endMarker) {
        receivedBytes[pos] = receivedByte;
        pos++;
        if (pos >= numBytes) {
          pos = numBytes - 1;
        }
      } else {
        recvInProgress = false;
        numReceived = pos;  // save the number for use when printing
        pos = 0;
        newData = true;
      }
    }

    else if (receivedByte == startMarker) {
      recvInProgress = true;
    }
  }
}

// Update the sequence of the desired instrument
void saveNewData() {
  if (newData == true && numReceived == 3) {
    // TODO adicionar os restantes instrumentos
    Serial.print("Received new sequence");
    for (int i = 0; i < 16; i++) {
      if (i < 8) {
        stepSeq.hiHat[i] = (receivedBytes[1] & (0x80 >> i)) ? true : false;
      } 
      else {
        stepSeq.hiHat[i] = (receivedBytes[2] & (0x80 >> (i - 8))) ? true : false;
      }
    }
    runSequencer = true;
  }
    newData = false;
}

// Get the OCR1A value from the number of milliseconds for the loop
uint16_t msToOcr1a(int milliseconds) {
  uint16_t result;
  result = MSTOCR1A_FACTOR * milliseconds - 1;
  return result;
}

void setup() {

  pinMode(13, OUTPUT);

  // set serial baud rate
  Serial.begin(115200);

  //////////////////////////////////////////////// SETTING TIMER INTERRUPT ////////////////////////////////

  cli();  //stop interrupts

  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   //initialize counter value to 0

  // set compare match register for 1hz increments
  OCR1A = msToOcr1a(500);  // set the milliseconds for the timer

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();  //allow interrupts
}  //end setup

// timer1 behaviour
ISR(TIMER1_COMPA_vect) {
  stepCounter++;
  if (stepCounter == 16) stepCounter = 0;
  stepChanged = true;
}

void loop() {
  if (Serial.available()>0) {
    recvSequence();
    saveNewData();
} 
  else if (runSequencer){
    if (stepChanged) {
      //TODO por todos os gpios a low e atualizar com o valor correto (avaliar se precisa de um timer)
      digitalWrite(13, LOW);
      delay(50);  // isto terá de ser relativo à frequência (que percentagem da duração do step fica a low)
      Serial.println(stepCounter);
      Serial.println(stepSeq.hiHat[stepCounter]);
      Serial.print("step: ");

      //digitalWrite(13, stepSeq.hiHat[stepCounter]);
      //Serial.println(stepSeq.cymbal[stepCounter]);
      //Serial.println(stepSeq.tomTom[stepCounter]);
      //Serial.println(stepSeq.snare[stepCounter]);
      //Serial.println(stepSeq.bassDrum[stepCounter]);
      //Serial.println(stepSeq.floorTom[stepCounter]);
      //Serial.println(stepSeq.hiHatFoot[stepCounter]);
      stepChanged = false;
    }
  }
}
