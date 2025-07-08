#include "serialcomm.h"
namespace serialcomm {

void SerialComm::showNewData() {
    if (newData == true) {
        Serial.print("This just in (HEX values)... ");
        for (byte n = 0; n < SERIAL_NUM_BYTES; n++) {
            Serial.print(receivedBytes[n], HEX);
            Serial.print(' ');
        }
        Serial.println();
        newData = false;
    }
}

void SerialComm::getBytesFromSerial() {
    static boolean receiving = false;
    static byte pos = 0;
    byte byteRead;

    while (Serial.available() > 0 && newData == false) {
        byteRead = Serial.read();
        if (receiving == true) {
            if (byteRead != endMarker) {
                receivedBytes[pos] = byteRead;
                pos++;
                if (pos >= SERIAL_NUM_BYTES) {
                    pos = SERIAL_NUM_BYTES - 1;
                }
            }
            else {
                // receivedBytes[pos] = '\0'; // terminate the string
                receiving = false;
                pos = 0;
                newData = true;
            }
        }
        else if (byteRead == startMarker) {
            receiving = true;
        }
    }
}
}