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
void loop()
{
	on = !on;

	if (on)
	{
		//Serial.print("AAA");
		slaveComms.print("AAA");
		digitalWrite(13, HIGH);
	}
	else
	{
		//Serial.print("   ");
		slaveComms.print("   ");
		digitalWrite(13, LOW);
	}

	delay(1000);
}
