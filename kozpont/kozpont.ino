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

// egyéb globális változók
long kuldemeny = 0L;
int ftomb[2];

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
		radio.read(ftomb, sizeof(ftomb));
		//radio.read(&kuldemeny, sizeof(kuldemeny));		// adatok beolvasása
		Serial.print(ftomb[0]);
		if (ftomb[0] == 1)
		{
			Serial.print(" bekapcs ag");
			Serial.println(ftomb[1]);
			digitalWrite(led, HIGH);
		}
		else
		{
			Serial.print(" kikapcs ag");
			Serial.println(ftomb[1]);
			digitalWrite(led, LOW);
		}
	}
	else
	{
		Serial.println("valami nem oke");
	}
	delay(10);
}


