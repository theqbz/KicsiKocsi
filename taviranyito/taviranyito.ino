/*
KICSIKOCSI TÁVIRÁNYÍTÓ

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

// Arduino Nano pin-kiosztás
const byte gomb = 4;
const byte led = 5;
const byte RfCS = 7;					// az nRF24L01 modul "Chip Set" lába
const byte RfCE = 8;					// az nRF24L01 modul "Chip Enable" lába
const byte MINT = 10;
const byte MSDA = A4;
const byte MSCL = A5;

// nRF24L01 rádió állandói
RF24 radio(RfCE, RfCS);					// Rádió létrehozása
const byte cim = 9654;					// a Rádió csatornájának címe

// egyéb globális változók
long uzenet = 0L;						// Be/Ki és az irányok egy 7 jegyû egész számban
int tomb[2];

void setup() {
	Serial.begin(9600);
	pinMode(gomb, INPUT);
	radio.begin();						// Rádió bekapcsolása
	radio.openWritingPipe(cim);			// csatorna megnyitása adatok küldéséhez a központba
	radio.setPALevel(RF24_PA_MIN);		// Rádió térerejének minimumra állítása
}


void loop() {
	radio.stopListening();							// adó-módba kapcsolja a rádiót

	if (digitalRead(gomb))
	{
		uzenet = 1L;
		tomb[0] = 1;
		tomb[1] = 12;
		radio.write(tomb, sizeof(tomb));
		//radio.write(&uzenet, sizeof(uzenet));		// ütenet küldése
		Serial.println("megnyomva: BE");
	}
	else
	{
		uzenet = 0L;
		tomb[0] = 2;
		tomb[1] = 22;
		radio.write(tomb, sizeof(tomb));
		//radio.write(&uzenet, sizeof(uzenet));		// ütenet küldése
		Serial.println("nincs megnyomva: KI");
	}
	delay(5);
}