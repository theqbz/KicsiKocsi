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
boolean gomb = 0;

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	radio.begin();
	radio.openReadingPipe(0, cim);
	radio.setPALevel(RF24_PA_MIN);
	radio.startListening();
}

void loop() {
	if (radio.available()) {
		char uzenet[32] = "";
		radio.read(&uzenet, sizeof(uzenet));
		radio.read(&gomb, sizeof(gomb));
		if (gomb == HIGH)
		{
			digitalWrite(LED_BUILTIN, HIGH);
			Serial.println(uzenet);
		}
		else
		{
			digitalWrite(LED_BUILTIN, LOW);
			Serial.println(uzenet);
		}
	}

	delay(5);
}


