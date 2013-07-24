// Port Hack Interactive Clock - Slave
// Copyright Daniel Tullemans 2013
#include <avr/io.h>
#include "WS2811.h"

#define BIT(B)           (0x01 << (uint8_t)(B))
#define SET_BIT_HI(V, B) (V) |= (uint8_t)BIT(B)
#define SET_BIT_LO(V, B) (V) &= (uint8_t)~BIT(B)

DEFINE_WS2811_FN(rgbOut, PORTB, 0)

#define BUFFER_LENGTH 20
#define PACKET_LENGTH 12

RGB_t rgbData[60];
char readBuffer[64];
char packet[PACKET_LENGTH];

void setup()
{
    SET_BIT_HI(DDRB, 0);
    SET_BIT_LO(PORTB, 0);

    memset(rgbData, 1, 180);
    memset(readBuffer, 0, 24);

    rgbOut(rgbData, 60);

    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);

    Serial.begin(9600);
}

void loop()
{
    if(Serial.available() >= BUFFER_LENGTH)
    {
        // Reading
        digitalWrite(13, HIGH);

        Serial.readBytes(readBuffer, Serial.available());

        // Done reading
        digitalWrite(13, LOW);

        char* readFrom = readBuffer;
        while (*readFrom == 0xFE)
            readFrom++;

        memcpy(packet, readFrom, PACKET_LENGTH);

        setClockHands(packet);
    }

    delay(10);
}

void setClockHands(char[] packet)
{
    
}
