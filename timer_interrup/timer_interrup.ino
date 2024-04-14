//this code will enable all three arduino timer interrupts.
//timer1 will interrupt at 1Hz

#define HIHAT_DEFAULT {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
#define CYMBAL_DEFAULT {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define TOMTOM_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}
#define SNARE_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}
#define FLOORTOM_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}
#define HIHATFOOT_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}
#define BASSDRUM_DEFAULT {1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1}

#define MSTOOCR1A_FACTOR (16000000.0)/(2000.0*1024.0)
//storage variables
bool toggle1 = 0;
uint8_t stepCounter = 0;
bool stepChanged = false;

// 
const byte numBytes = 32;
byte receivedBytes[numBytes];
byte numReceived = 0;

boolean newData = false;

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


// Get the OCR1A value from the number of milliseconds for the loop
uint16_t msToOcr1a(int milliseconds){
  uint16_t result;
  result = MSTOOCR1A_FACTOR*milliseconds-1;
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
  OCR1A = msToOcr1a(500); // set the milliseconds for the timer
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();  //allow interrupts
}  //end setup

// Create a signal receiver class for this function
void recvBytesWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    byte startMarker = 0x3C;
    byte endMarker = 0x3E;
    byte rb;
   

    while (Serial.available() > 0 && newData == false) {
        rb = Serial.read();

        if (recvInProgress == true) {
            if (rb != endMarker) {
                receivedBytes[ndx] = rb;
                ndx++;
                if (ndx >= numBytes) {
                    ndx = numBytes - 1;
                }
            }
            else {
                receivedBytes[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                numReceived = ndx;  // save the number for use when printing
                ndx = 0;
                newData = true;
            }
        }

        else if (rb == startMarker) {
            recvInProgress = true;
        }
    }
}

// Create a signal receiver class for this function
void showNewData() {
    if (newData == true) {
        Serial.print("This just in (HEX values)... ");
        for (byte n = 0; n < numReceived; n++) {
            Serial.print(receivedBytes[n], HEX);
            Serial.print(' ');
        }
        Serial.println();
        newData = false;
    }
}

ISR(TIMER1_COMPA_vect) {  //timer1 interrupt 1Hz toggles pin 13 (LED)
                          //generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  stepCounter++;
  if (stepCounter == 16) stepCounter = 0;
  stepChanged = true;
}

void loop() {
  recvBytesWithStartEndMarkers();
  showNewData();
  
  if(stepChanged){
    //TODO por todos os gpios a low e atualizar com o valor correto (avaliar se precisa de um timer)
    digitalWrite(13, LOW);
    delay(50); // isto terá de ser relativo á frequência (que percentagem da duração do step fica a low)
    Serial.println("^_^");
    Serial.println(stepCounter);
    Serial.println(stepSeq.hiHat[stepCounter]);   
    digitalWrite(13, stepSeq.hiHat[stepCounter]);
    Serial.println(stepSeq.cymbal[stepCounter]);   
    Serial.println(stepSeq.tomTom[stepCounter]);   
    Serial.println(stepSeq.snare[stepCounter]);   
    Serial.println(stepSeq.bassDrum[stepCounter]);   
    Serial.println(stepSeq.floorTom[stepCounter]);   
    Serial.println(stepSeq.hiHatFoot[stepCounter]);
    stepChanged=false;
  }
}
