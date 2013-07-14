// Port Hack Interactive Clock - Slave
// Copyright Daniel Tullemans 2013
#include <avr/io.h>
#include "WS2811.h"

#define BIT(B)           (0x01 << (uint8_t)(B))
#define SET_BIT_HI(V, B) (V) |= (uint8_t)BIT(B)
#define SET_BIT_LO(V, B) (V) &= (uint8_t)~BIT(B)

DEFINE_WS2811_FN(rgbOut, PORTB, 0)

RGB_t rgbData[60];
char readBuffer[180];

void setup()
{
    SET_BIT_HI(DDRB, 0);
    SET_BIT_LO(PORTB, 0);

	memset(rgbData, 1, 180);
	memset(readBuffer, 0, 180);

	rgbOut(rgbData, 60);

	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	Serial.begin(9600);
}


void loop()
{
	if(Serial.available() >= 180)
	{
		digitalWrite(13, HIGH);

		memset(rgbData, 0, 180);

		Serial.readBytes(readBuffer, 180);
		memcpy(rgbData, readBuffer, 180);

		rgbOut(rgbData, 60);

		digitalWrite(13, LOW);
	}

	delay(10);
}