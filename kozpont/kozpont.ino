/*
KICSIKOCSI K�PONTI EGYS�G

 Name:		kozpont.ino
 Created:	12/5/2020 8:17:27 PM
 Author:	qbz
*/

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

// Arduino UNO Wifi R2 pin-kioszt�s
const byte led = 2;
const byte RfCS = 8;					// az nRF24L01 modul "Chip Set" l�ba
const byte RfCE = 7;					// az nRF24L01 modul "Chip Enable" l�ba

// nRF24L01 r�di� �lland�i
RF24 radio(RfCE, RfCS);					// R�di� l�trehoz�sa
const byte cim = 9654;					// a R�di� csatorn�j�nak c�me


void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	radio.begin();						// R�di� bekapcsol�sa
	radio.openReadingPipe(0, cim);		// csatorna megnyit�sa adatok fogad�s�hoz a t�vir�ny�t�t�l
	radio.setPALevel(RF24_PA_MIN);		// R�di� t�rerej�nek minimumra �ll�t�sa
}

void loop() {
	radio.startListening();								// vev�-m�dba kapcsolja a r�di�t

	if (radio.available())								// ha van fogadott adat (amit a t�vir�ny�t� k�ld�tt)
	{
		int fogadott = 0;
		radio.read(&fogadott, sizeof(fogadott));		// adatok beolvas�sa
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


