// Port Hack Interactive Clock - Master
// Copyright Daniel Tullemans 2013
#include <SoftwareSerial.h>

SoftwareSerial slaveComms(10, 11);

#define BUFFER_LENGTH 20
#define START_PADDING 0
#define END_PADDING 8
#define DATA_LENGTH (BUFFER_LENGTH - START_PADDING - END_PADDING)

unsigned char writeBuffer[BUFFER_LENGTH];
unsigned char *packetData = writeBuffer + START_PADDING;

void setup()
{
    delay(500);
    slaveComms.begin(9600); 

    pinMode(13, OUTPUT);

    memset(writeBuffer, 0xFE, BUFFER_LENGTH);
}

bool on = false;

unsigned char onData[DATA_LENGTH] = {
    0x01, 0x01, 0x01,
    0x30, 0x00, 0x00,
    0x00, 0x30, 0x00,
    0x00, 0x00, 0x30
};
unsigned char offData[DATA_LENGTH] = {
    0x07, 0x07, 0x07,
    0x07, 0x07, 0x07,
    0x07, 0x07, 0x07,
    0x07, 0x07, 0x07
};

void loop()
{
    if (on = !on)
    {
        memcpy(packetData, onData, DATA_LENGTH);
    }
    else
    {
        memcpy(packetData, offData, DATA_LENGTH);
    }

    digitalWrite(13, HIGH);
    slaveComms.write(writeBuffer, BUFFER_LENGTH);
    digitalWrite(13, LOW);

    delay(700);
}
