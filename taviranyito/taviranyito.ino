/*
KICSIKOCSI T�VIR�NY�T�

 Name:		kicsikocsi.ino
 Created:	12/5/2020 6:14:38 PM
 Author:	qbz
*/

#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <MPU6050.h>

// Arduino Nano pin-kioszt�s
const byte led = 3;
const byte gomb = 9;
const byte RfCS = 7;					// az nRF24L01 modul "Chip Set" l�ba
const byte RfCE = 8;					// az nRF24L01 modul "Chip Enable" l�ba
const byte MINT = 2;
const byte MSDA = A4;
const byte MSCL = A5;

// nRF24L01 r�di� �lland�i
RF24 radio(RfCE, RfCS);					// R�di� l�trehoz�sa
const byte cim = 9654;					// a R�di� csatorn�j�nak c�me

// egy�b glob�lis v�ltoz�k
char uzenet[] = "Egyelore semmi nem tortent";

void setup() {
	Serial.begin(9600);
	pinMode(gomb, INPUT);
	radio.begin();						// R�di� bekapcsol�sa
	radio.openWritingPipe(cim);			// csatorna megnyit�sa adatok k�ld�s�hez a k�zpontba
	radio.setPALevel(RF24_PA_MIN);		// R�di� t�rerej�nek minimumra �ll�t�sa
	radio.stopListening();				// ad�-m�dba kapcsolja a r�di�t
}


void loop() {
	if (digitalRead(gomb))
	{
		int uzenet = 1;
		radio.write(&uzenet, sizeof(uzenet));		// �tenet k�ld�se
		Serial.println("megnyomva: BE");
	}
	else
	{
		int uzenet = 0;
		radio.write(&uzenet, sizeof(uzenet));		// �tenet k�ld�se
		Serial.println("nincs megnyomva: KI");
	}
	delay(5);
}