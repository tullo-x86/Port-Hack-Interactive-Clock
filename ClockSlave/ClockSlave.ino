// Port Hack Interactive Clock - Slave
// Copyright Daniel Tullemans 2013
#include <avr/io.h>
#include "WS2811.h"

#include <Time.h>

#define BIT(B)           (0x01 << (uint8_t)(B))
#define SET_BIT_HI(V, B) (V) |= (uint8_t)BIT(B)
#define SET_BIT_LO(V, B) (V) &= (uint8_t)~BIT(B)

#define BUFFER_LENGTH 20
#define PACKET_LENGTH 12

DEFINE_WS2811_FN(WS2811RGB, PORTB, 0)

RGB_t rgbData[60];
char readBuffer[64];
char packetBuffer[PACKET_LENGTH];

const unsigned char BRIGHTNESS = 32;

void setup()
{
    SET_BIT_HI(DDRB, 0);
    SET_BIT_LO(PORTB, 0);

    memset(rgbData, 1, 180);
    memset(readBuffer, 0, 24);

    WS2811RGB(rgbData, 60);

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

        memcpy(packetBuffer, readFrom, 4);

        setClockHands(packetBuffer);
    }

    delay(100);
}

void setClockHands(char *packet)
{
    unsigned long theTime = *packet;

    for (int i = 0; i < 60; ++i)
    {
        rgbData[i].r = 0;
        rgbData[i].g = 0;
        rgbData[i].b = 0;
    }

    int theSecond = second(theTime);
    int secondPosition = theSecond;
    rgbData[secondPosition].r = BRIGHTNESS;
    
    int theMinute = minute(theTime);
    int minutePosition = theMinute;
    rgbData[minutePosition].g = BRIGHTNESS;
    
    int theHour = hour(theTime);
    int hourPosition = (
        (theHour * 5) + (theMinute / 12)
      ) % 60;
    rgbData[hourPosition].b = BRIGHTNESS;

    WS2811RGB(rgbData, 60);
}
