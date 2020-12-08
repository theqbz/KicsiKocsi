/*
KICSIKOCSI TÁVIRÁNYÍTÓ

 Name:		kicsikocsi.ino
 Created:	12/5/2020 6:14:38 PM
 Author:	qbz
*/

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <MPU6050.h>

#define XminNyers -16000		// a nyers adatok alsó határa levágáshoz
#define XmaxNyers 16000			// a nyers adatok felsõ határa levágáshoz
#define YminNyers -16000		// a nyers adatok alsó határa levágáshoz
#define YmaxNyers 16000			// a nyers adatok felsõ határa levágáshoz
#define minSeb 0				// a konverzió alsó határa sebességhez
#define maxSeb 255				// a konverzió felsõ határa sebességhez
#define minSzog 1				// a konverzió alsó határa kormány-szöghöz
#define maxSzog 179				// a konverzió felsõ határa kormány-szöghöz

// Arduino Nano pin-kiosztás
#define gomb 4
#define led 5
#define RfCS 7					// az nRF24L01 modul "Chip Set" lába
#define RfCE 8					// az nRF24L01 modul "Chip Enable" lába
#define MINT 10
#define MSDA A4
#define MSCL A5

// nRF24L01 rádió állandói
RF24 radio(RfCE, RfCS);					// Rádió
const byte cim = 9654;					// a Rádió csatornájának címe

// MPU6050 giroszkóp állandói
MPU6050 giroszkop;						// Giroszkóp
int16_t nyAx, nyAy, nyAz,nyGx, nyGy, nyGz;				// nyers adatok a giroszkópról

// egyéb globális változók
byte csomag[3] = { 0,0,0 };				// Egy adatcsomag: elõre, oldalra, ok
int x = 0;								// döntés elõre-hátra 
int y = 0;								// oldalirányú dõlés (0-90-ig: jobbra, 90-180-ig balra)

void setup() {
	//Serial.begin(9600);
	giroszkop.initialize();				// Giroszkóp indítása
	pinMode(gomb, INPUT);
	radio.begin();						// Rádió bekapcsolása
	radio.openWritingPipe(cim);			// csatorna megnyitása adatok küldéséhez a központba
	radio.setPALevel(RF24_PA_MIN);		// Rádió térerejének minimumra állítása
	GiroOffset();						// Giroszkóp nullázása
}


void loop() {
	giroszkop.getMotion6(&nyAx, &nyAy, &nyAz, &nyGx, &nyGy, &nyGz);			// irányok kiolvasása
	int cAx = constrain(nyAx, XminNyers, XmaxNyers);						// értékek beszorítása a [0, 16000] intervallumba
	int cAy = constrain(nyAy, YminNyers, YmaxNyers);			
	csomag[0] = map(cAx, XminNyers, XmaxNyers, minSeb, maxSeb);				// sebesség érték konvertálása a [0, 255] intervallumba
	csomag[1] = map(cAy, YminNyers, YmaxNyers, minSzog, maxSzog);			// irány konvertálása a [0,180] intervallumba
	if (digitalRead(gomb))
	{
		csomag[2] = 1;
	}
	else
	{
		csomag[2] = 1;
	}
	
	//delay(5);

	radio.stopListening();													// adó-módba kapcsolja a rádiót
	radio.write(&csomag, sizeof(csomag));									// ütenet küldése
}


void GiroOffset() {
	giroszkop.setXAccelOffset(-549);
	giroszkop.setYAccelOffset(-5894);
	giroszkop.setZAccelOffset(1684);
}

