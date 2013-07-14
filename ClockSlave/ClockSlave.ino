// Port Hack Interactive Clock - Slave
// Copyright Daniel Tullemans 2013
#include <avr/io.h>
#include "WS2811.h"

#define BIT(B)           (0x01 << (uint8_t)(B))
#define SET_BIT_HI(V, B) (V) |= (uint8_t)BIT(B)
#define SET_BIT_LO(V, B) (V) &= (uint8_t)~BIT(B)

DEFINE_WS2811_FN(rgbOut, PORTB, 0)

RGB_t rgbData[60];

void setup()
{
    SET_BIT_HI(DDRB, 0);
    SET_BIT_LO(PORTB, 0);

	for (int i = 0; i < 60; ++i)
	{
		rgbData[i].r = rgbData[i].g = rgbData[i].b = 32;
	}
	rgbOut(rgbData, 60);

	pinMode(13, OUTPUT);

	Serial.begin(9600);
}

void loop()
{
	char readBuffer[1];
	
	if(Serial.available() > 0)
	{
		digitalWrite(13, HIGH);

		Serial.readBytes(readBuffer, 1);

		for (int i = 0; i < 60; ++i)
		{
			rgbData[i].r = rgbData[i].g = rgbData[i].b = readBuffer[0];
		}

		rgbOut(rgbData, 60);

		digitalWrite(13, LOW);
	}

	delay(10);
}