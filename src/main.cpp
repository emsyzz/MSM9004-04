#include <Arduino.h>

#define PIN_DATA 9
#define PIN_CLOCK 8
#define PIN_LOAD 10

#define DATA_BITS 50
#define DELAY_US 1

void bangBit(bool bit) {
    digitalWrite(PIN_DATA, bit);
    digitalWrite(PIN_CLOCK, HIGH);
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_DATA, LOW);
    digitalWrite(13, !digitalRead(13));
}

void sendDataBit(int data_bit, int com) {
    Serial.print("Bit #");
    Serial.print(data_bit);
    Serial.print("; COM #");
    Serial.print(com);
    Serial.print("\n");
    int i, j, k;
    for (j = 1; j <= 4; j++) {
        for (i = DATA_BITS; i > 0; i--) {
            bangBit(data_bit == i && com == j ? 1 : 0);
            /*Serial.print("Sending bit #");
            Serial.print(i);
            Serial.print("; ");
            Serial.print(value, DEC);
            Serial.print("\n");*/
        }
        bangBit(0);
        bangBit(0);

        for (k = 1; k <= 4; k++) {
            bangBit(com == j && k == com);
            /*Serial.print("Sending COM #");
            Serial.print(k);
            Serial.print("; ");
            Serial.print(k == j, DEC);
            Serial.print("\n");*/
        }

        digitalWrite(PIN_LOAD, HIGH);
        digitalWrite(PIN_LOAD, LOW);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_DATA, OUTPUT);
    pinMode(PIN_LOAD, OUTPUT);
    pinMode(PIN_CLOCK, OUTPUT);
    pinMode(13, OUTPUT);
    digitalWrite(PIN_DATA, LOW);
    digitalWrite(PIN_LOAD, LOW);
    digitalWrite(PIN_CLOCK, LOW);
}

static byte devnull;
void loop() {
    int i, j;
    for (j = 1; j <= 4; j++) {
        for(i = 1; i <= DATA_BITS; i++) {
            sendDataBit(i, j);
            while (!Serial.available());
            while (Serial.available()) {
                devnull = Serial.read();
                delay(100);
            }
        }
    }
}
