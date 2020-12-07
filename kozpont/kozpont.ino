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

// egy�b glob�lis v�ltoz�k
long kuldemeny = 0L;
int ftomb[2];

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
		radio.read(ftomb, sizeof(ftomb));
		//radio.read(&kuldemeny, sizeof(kuldemeny));		// adatok beolvas�sa
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


