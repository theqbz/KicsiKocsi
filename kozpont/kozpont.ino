/*
KICSIKOCSI KÖPONTI EGYSÉG

 Name:		kozpont.ino
 Created:	12/5/2020 8:17:27 PM
 Author:	qbz
*/

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

// Arduino UNO Wifi R2 pin-kiosztás
const byte led = 2;
const byte RfCS = 8;					// az nRF24L01 modul "Chip Set" lába
const byte RfCE = 7;					// az nRF24L01 modul "Chip Enable" lába

// nRF24L01 rádió állandói
RF24 radio(RfCE, RfCS);					// Rádió létrehozása
const byte cim = 9654;					// a Rádió csatornájának címe


void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	radio.begin();						// Rádió bekapcsolása
	radio.openReadingPipe(0, cim);		// csatorna megnyitása adatok fogadásához a távirányítótól
	radio.setPALevel(RF24_PA_MIN);		// Rádió térerejének minimumra állítása
}

void loop() {
	radio.startListening();								// vevõ-módba kapcsolja a rádiót

	if (radio.available())								// ha van fogadott adat (amit a távirányító küldött)
	{
		int fogadott = 0;
		radio.read(&fogadott, sizeof(fogadott));		// adatok beolvasása
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
}


