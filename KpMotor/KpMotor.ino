/*
  Name:		MOTOR (KICSIKOCSI KOZPONTI EGYSEG)
  Created:	1/9/2021 7:59:35 PM
  Author:	qbz
*/

#include <SPI.h>
#include <RF24_config.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
#include <Wire.h>
#include <Servo.h>
//#include "Kocsi.h"


// Arduino Nano pin kiosztas
#define MOTOR_ELORE 3			// L293 "Input 1" (elore forog a motor)
#define MOTOR_HATRA 4			// L293 "Input 2" (hatra forog a motor)
#define SERVO 5					// A szervo motor vezerloje
#define MOTOR_ENABLE 6			// L293 "Enable" (motor bekapcsolasa)
#define RF_CE 7					// nRF24L01 "Chip Enable"
#define RF_CS 8					// nRF24L01 "Chip Set"
#define PIEZZO 9				// csipogo
#define RF_MOSI 11				// nRF24L01 MOSI (Az ICSP kommunikaciohoz szukseges)
#define RF_MISO 12				// nRF24L01 MISO (Az ICSP kommunikaciohoz szukseges)
#define RF_SCK 13				// nRF24L01 SCK (Az ICSP kommunikaciohoz szukseges)

#define TIMEOUT 100				// riasztas, ha ennyi cikluson át nem uzen a taviranyito
#define AKADALY 10				// ennel kozelebb (centimeterben)
#define MIN_SEBESSEG 100		// limit alatti erteknel nem indul a motor


RF24 radio(RF_CE, RF_CS);		// Radio letrehozasa
Servo kormany;					// Kormany letrehozasa

byte kontrollerSebesseg;		// A taviranyito altal kuldott sebesseg adat
byte kontrollerKormany;			// A taviranyito altal kuldott kormanyzas adat
bool kontrollerGomb;			// A taviranyito gombjanak allapota
int kontrollerOfflineTimer;		// A taviranyito offline allapotanak kesleltetese
bool kontrollerOnline;			// A taviranyito online allapota
byte elsoLokator;				// Az elso lokator altal mert tavolsag
byte hatsoLokator;				// A hatso lokator altal mert tavolsag

void mozgas()
{
	lokatorOlvasasa();
	kontrollerOlvasasa();
	kormany.write(kontrollerKormany);
	analogWrite(MOTOR_ENABLE, sebessegSzamitasa());
}

void lokatorOlvasasa()
{
	Wire.requestFrom(0x02, 2);
	if (Wire.available()) {
		elsoLokator = Wire.read();
		hatsoLokator = Wire.read();
	}
}

void kontrollerOlvasasa()
{
	radio.startListening();
	if (radio.available()) radioAdatokFeldolgozasa();
	else kontrollerOfflineTimer++;
	if (kontrollerOfflineTimer == TIMEOUT) nincsKapcsolat();
}

void radioAdatokFeldolgozasa()
{
	if (!kontrollerOnline) lettKapcsolat();
	byte csomag[3];
	radio.read(&csomag, sizeof(csomag));
	kontrollerOfflineTimer = 0;
	kontrollerSebesseg = csomag[0];
	kontrollerKormany = csomag[1];
	kontrollerGomb = csomag[2];
}

void nincsKapcsolat()
{
	tone(PIEZZO, 1000, 40);
	delay(100);
	tone(PIEZZO, 880, 20);
	delay(100);
	tone(PIEZZO, 880, 20);
	kontrollerOnline = false;
	kontrollerOfflineTimer = 0;
}

void lettKapcsolat()
{
	tone(PIEZZO, 880, 40);
	delay(250);
	tone(PIEZZO, 1760, 40);
	kontrollerOnline = true;
}

int sebessegSzamitasa()
{
	if (kontrollerGomb == 1 && kontrollerOnline) {
		int sebesseg = map(kontrollerSebesseg, 0, 255, -255, 255);
		if (sebesseg < -MIN_SEBESSEG) {									// ha a sebesseg alapjan hatrament van es nagyobb a minimumnal
			digitalWrite(MOTOR_ELORE, LOW);
			digitalWrite(MOTOR_HATRA, HIGH);
			if (hatsoLokator < AKADALY) sebesseg = 0;					// ha a hatso lokotor alapjan kozel egy akadaly
			else sebesseg = -sebesseg;									// a negativ sebessegertek abszoluterteke
		}
		else if (sebesseg > MIN_SEBESSEG) {								// ha eloremenet van es a sebesseg meghaladja a minimumiot
			digitalWrite(MOTOR_ELORE, HIGH);
			digitalWrite(MOTOR_HATRA, LOW);
			if (elsoLokator < AKADALY) sebesseg = 0;					// ha az elso lokotor alapjan kozel egy akadaly
		}
		else sebesseg = 0;
		return sebesseg;
	}
	else return 0;
}

//void debug()
//{
//	Serial.print("K[0]: ");
//	Serial.print(kontrollerSebesseg);
//	Serial.print(" K[1]: ");
//	Serial.print(kontrollerKormany);
//	Serial.print(" K[2]: ");
//	Serial.print(kontrollerGomb);
//	Serial.print(" OTime: ");
//	Serial.print(kontrollerOfflineTimer);
//	Serial.print(" KOnline: ");
//	Serial.print(kontrollerOnline);
//	Serial.print(" ELok: ");
//	Serial.print(elsoLokator);
//	Serial.print(" HLok: ");
//	Serial.println(hatsoLokator);
//}

void setup()
{
	pinMode(MOTOR_ELORE, OUTPUT);
	pinMode(MOTOR_HATRA, OUTPUT);
	pinMode(MOTOR_ENABLE, OUTPUT);
	pinMode(PIEZZO, OUTPUT);
	Serial.begin(9600);
	Wire.begin();
	radio.begin();                          // Radio bekapcsolasa
	radio.openReadingPipe(0, 96);			// csatorna nyitasa adatok fogadasahoz a taviranyitotol
	radio.setPALevel(RF24_PA_MIN);			// Radio tererejenek minimumra allitasa
	kormany.attach(SERVO);					// Szervo csatlakoztatasa
	kormany.write(90);						// kormany kiegyenesitese
}

void loop()
{
	mozgas();
}
