// Port Hack Interactive Clock - Master
// Copyright Daniel Tullemans 2013
#include <SoftwareSerial.h>

SoftwareSerial slaveComms(10, 11);

void setup()
{
	Serial.begin(9600); // Serial to PC
	Serial.println("Initializing slave");

	slaveComms.begin(9600);

	pinMode(13, OUTPUT);
}

bool on = false;
void loop()
{
	on = !on;

	if (on)
	{
		Serial.write(127);
		slaveComms.write(127);
		digitalWrite(13, HIGH);
	}
	else
	{
		Serial.write(1);
		slaveComms.write(1);
		digitalWrite(13, LOW);
	}

	delay(1000);
}
