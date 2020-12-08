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

#define XminNyers -16000
#define XmaxNyers 16000
#define YminNyers -16000
#define YmaxNyers 16000
#define minSzog 0
#define maxSzog 180

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
int16_t nyAx, nyAy, nyAz;				// nyers gyorsulás adatok az adott tengelyen
int16_t nyGx, nyGy, nyGz;				// nyers elfordulás adatok az adott tengelyen

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
	csomag[0] = map(cAx, XminNyers, XmaxNyers, minSzog, maxSzog);			// limitált érték konvertálása a [0, 180] intervallumba
	csomag[1] = map(cAy, YminNyers, YmaxNyers, minSzog, maxSzog);
	if (digitalRead(gomb))
	{
		csomag[2] = 1;
	}
	else
	{
		csomag[2] = 0;
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

