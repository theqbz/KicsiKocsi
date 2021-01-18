/*
 Name:		LOKATOR (KICSIKOCSI KOZPONTI EGYSEG)
		Arduino UNO R3
*/

#include <Wire.h>
#include "Lokator.h"

// Arduino UNO R3 pin kiosztas
#define HC1Echo 4				// Hatso HC-SR04-4P Echo
#define HC1Trig 5				// Hatso HC-SR04-4P Triger
#define HC2Echo 6				// Elso HC-SR04 Echo
#define HC2Trig 7				// Elso HC-SR04 Triger

// egyeb allandok
#define LOKATOROK_SZAMA 2										// A kocsin mukodo lokatorok szama
#define FOGADOTT_TOMB_MERET 3								// A masik arduinotol fogadott adatok merete

byte kuldottAdatok[LOKATOROK_SZAMA];
byte fogadottAdatok[FOGADOTT_TOMB_MERET];
bool adatFogadasRendben = false;
Lokator elso(HC2Trig, HC2Echo);
Lokator hatso(HC1Trig, HC1Echo);

// debug
unsigned long currentTime;
unsigned long previousTime;
int L;

void adatokFogadasa(int n) {
	if (n != FOGADOTT_TOMB_MERET) digitalWrite(LED_BUILTIN, HIGH);
	else {
		digitalWrite(LED_BUILTIN, LOW);
		int i = 0;
		while (i < FOGADOTT_TOMB_MERET && Wire.available()) {
			fogadottAdatok[i] = Wire.read();
			i++;
		}
	}
}

void adatokKuldese() {
	kuldottAdatok[0] = elso.getTavolsag();
	kuldottAdatok[1] = hatso.getTavolsag();
	Wire.write(kuldottAdatok, sizeof(kuldottAdatok));
}

void setup() {
	pinMode(HC1Trig, OUTPUT);
	pinMode(HC1Echo, INPUT);
	pinMode(HC2Trig, OUTPUT);
	pinMode(HC2Echo, INPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	Wire.begin(0x02);
	Wire.onReceive(adatokFogadasa);
	Wire.onRequest(adatokKuldese);
}

void loop() {

// debug
	currentTime = millis();
	L = currentTime - previousTime;
	previousTime = currentTime;
}

