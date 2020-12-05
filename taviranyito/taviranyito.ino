/*
 Name:		kicsikocsi.ino
 Created:	12/5/2020 6:14:38 PM
 Author:	qbz
*/

//KICSIKOCSI TÁVIRÁNYÍTÓ

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

RF24 radio(RfCE, RfCS);					// adó beállítása
const byte adas = 1;					// cím adáshoz
const byte vetel = 2;					// cím vételhez

void setup() {
	pinMode(gomb, INPUT);
	pinMode(led, OUTPUT);
	radio.begin();						// adó indítása
	radio.openWritingPipe(adas);		// csatorna beállítása küldéshez
	radio.openReadingPipe(1, vetel);	// csatorna beállítása vételhez
	radio.setPALevel(RF24_PA_MIN);		// adó térereje
}

void loop() {
	radio.stopListening();										// beállítás adónak
	ezmegnyomva = digitalRead(gomb);
	radio.write(&ezmegnyomva, sizeof(ezmegnyomva));				// gomb állásának küldése
	delay(5);

	radio.startListening();										// beállítás vevõnek
	while (!radio.available());									// figyeli, hogy jön-e adás
	radio.read(&fogadottutasitas, sizeof(fogadottutasitas));	// kiolvassa az adást
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