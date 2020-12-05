/*
 Name:		kicsikocsi.ino
 Created:	12/5/2020 6:14:38 PM
 Author:	qbz
*/

//KICSIKOCSI TÁVIRÁNYÍTÓ


#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <MPU6050.h>

const byte RfCS = 7;
const byte RfCE = 8;
const byte RfMOSI = 11;
const byte RfMISO = 12;
const byte RfSCK = 13;

const byte MINT = 2;
const byte MSDA = A4;
const byte MSCL = A5;

const byte gomb = 9;
boolean megnyomva = 0;
byte allapot = 0;

const byte led = 3;

RF24 radio(RfCE, RfCS);
const byte cim = 00001;

void setup() {
	Serial.begin(9600);
	pinMode(gomb, INPUT);
	radio.begin();
	radio.openWritingPipe(cim);
	radio.setPALevel(RF24_PA_MIN);
	radio.stopListening();
}


void loop() {
	megnyomva = digitalRead(gomb);
	Serial.print("megnyomva: ");
	Serial.println(megnyomva);
	if (megnyomva == HIGH)
	{
		const char szoveg[] = "A gomb meg van nyomva";
		radio.write(&szoveg, sizeof(szoveg));
		allapot = 1;
	}
	else
	{
		const char szoveg[] = "A gomb nincs megnyomva";
		radio.write(&szoveg, sizeof(szoveg));
		allapot = 2;
	}
	radio.write(&allapot, sizeof(allapot));
	delay(300);
}