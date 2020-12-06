/*
 Name:		kozpont.ino
 Created:	12/5/2020 8:17:27 PM
 Author:	qbz
*/

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(7, 8);
const byte cim = 9654;
const byte led = 2;


//long gomb = 0;

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	radio.begin();
	radio.openReadingPipe(0, cim);
	radio.setPALevel(RF24_PA_MIN);
	radio.startListening();
}

void loop() {
	if (radio.available())
	{
		int fogadott = 0;
		radio.read(&fogadott, sizeof(fogadott));
		Serial.print(fogadott);
		if (fogadott == 1)
		{
			Serial.println(" bekapcs ag");
			digitalWrite(led, HIGH);
		}
		else
		{
			Serial.println(" kikapcs ag");
			digitalWrite(led, LOW);
		}
	}
	else
	{
		Serial.println("valami nem oke");
	}
	delay(10);

		//radio.read(&gomb, sizeof(gomb));
		//Serial.print("Gomb a Nano-n megnyomva: ");
		//Serial.print(gomb);
		//long negyzet = gomb * gomb;
		//if (gomb == HIGH)
		//{
		//	digitalWrite(led, LOW);
		//	Serial.print("Van uzenet: ");
		//	Serial.println(uzenet);
		//}
		//else
		//{
		//	digitalWrite(led, HIGH);
		//	Serial.print("Nincs uzenet: ");
		//	Serial.println(uzenet);
		//}
}


