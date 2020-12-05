/*
 Name:		kicsikocsi.ino
 Created:	12/5/2020 6:14:38 PM
 Author:	qbz
*/

//KICSIKOCSI T�VIR�NY�T�

//const byte RfMOSI = 11;
//const byte RfMISO = 12;
//const byte RfSCK = 13;
//
//const byte MINT = 2;
//const byte MSDA = A4;
//const byte MSCL = A5;


#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <MPU6050.h>

const byte RfCS = 7;					// "chip select" pin
const byte RfCE = 8;					// "chip enable" pin

const byte gomb = 9;
const byte led = 3;

boolean ezmegnyomva = 0;
int fogadottutasitas = 0;

RF24 radio(RfCE, RfCS);					// ad� be�ll�t�sa
const byte adas = 1;					// c�m ad�shoz
const byte vetel = 2;					// c�m v�telhez

void setup() {
	pinMode(gomb, INPUT);
	pinMode(led, OUTPUT);
	radio.begin();						// ad� ind�t�sa
	radio.openWritingPipe(adas);		// csatorna be�ll�t�sa k�ld�shez
	radio.openReadingPipe(1, vetel);	// csatorna be�ll�t�sa v�telhez
	radio.setPALevel(RF24_PA_MIN);		// ad� t�rereje
}

void loop() {
	radio.stopListening();										// be�ll�t�s ad�nak
	ezmegnyomva = digitalRead(gomb);
	radio.write(&ezmegnyomva, sizeof(ezmegnyomva));				// gomb �ll�s�nak k�ld�se
	delay(5);

	radio.startListening();										// be�ll�t�s vev�nek
	while (!radio.available());									// figyeli, hogy j�n-e ad�s
	radio.read(&fogadottutasitas, sizeof(fogadottutasitas));	// kiolvassa az ad�st
	if (fogadottutasitas == 1)
	{
		digitalWrite(led, HIGH);
//		const char szoveg[] = "BE";
//		radio.write(&szoveg, sizeof(szoveg));
	}
	else
	{
		digitalWrite(led, LOW);
//		const char szoveg[] = "KI";
//		radio.write(&szoveg, sizeof(szoveg));
	}
	delay(5);
}