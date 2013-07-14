// Port Hack Interactive Clock - Master
// Copyright Daniel Tullemans 2013
#include <SoftwareSerial.h>

SoftwareSerial slaveComms(10, 11);

void setup()
{
	//Serial.begin(9600); // Serial to PC
	//Serial.println("Initializing slave");

	slaveComms.begin(9600);

	pinMode(13, OUTPUT);
}

bool on = false;
const uint8_t bright[9] = { 255, 0, 0, 0, 255, 0, 0, 0, 255};
const uint8_t dim[9] = { 32, 0, 0, 0, 32, 0, 0, 0, 32};
void loop()
{
	on = !on;

	if (on)
	{
		slaveComms.write(bright, 9);
		digitalWrite(13, HIGH);
	}
	else
	{
		slaveComms.write(dim, 9);
		digitalWrite(13, LOW);
	}

	delay(100);
}
