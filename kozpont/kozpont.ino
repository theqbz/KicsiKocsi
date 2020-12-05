/*
 Name:		kozpont.ino
 Created:	12/5/2020 8:17:27 PM
 Author:	qbz
*/

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(7, 8);
const byte cim = 00001;
byte gomb = 3;
const byte led = 2;

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	radio.begin();
	radio.openReadingPipe(0, cim);
	radio.setPALevel(RF24_PA_MIN);
	radio.startListening();
	digitalWrite(led, HIGH);
}

void loop() {
	if (radio.available()) {
		char uzenet[32] = "";
		radio.read(&uzenet, sizeof(uzenet));
		delay(5);
		radio.read(&gomb, sizeof(gomb));
		if (gomb == 1)
		{
			digitalWrite(led, LOW);
			Serial.print("Van uzenet: ");
			Serial.println(uzenet);
			Serial.print("Gomb: ");
			Serial.println(gomb);
		}
		else
		{
			digitalWrite(led, HIGH);
			Serial.print("Nincs uzenet: ");
			Serial.println(uzenet);
			Serial.print("Gomb: ");
			Serial.println(gomb);
		}
	}

	delay(5);
}


