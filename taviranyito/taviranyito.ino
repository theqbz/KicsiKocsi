/*
KICSIKOCSI T�VIR�NY�T�

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

// Arduino Nano pin-kioszt�s
#define gomb 4
#define led 5
#define RfCS 7					// az nRF24L01 modul "Chip Set" l�ba
#define RfCE 8					// az nRF24L01 modul "Chip Enable" l�ba
#define MINT 10
#define MSDA A4
#define MSCL A5

// nRF24L01 r�di� �lland�i
RF24 radio(RfCE, RfCS);					// R�di�
const byte cim = 9654;					// a R�di� csatorn�j�nak c�me

// MPU6050 giroszk�p �lland�i
MPU6050 giroszkop;						// Giroszk�p
int16_t nyAx, nyAy, nyAz;				// nyers gyorsul�s adatok az adott tengelyen
int16_t nyGx, nyGy, nyGz;				// nyers elfordul�s adatok az adott tengelyen

// egy�b glob�lis v�ltoz�k
byte csomag[3] = { 0,0,0 };				// Egy adatcsomag: el�re, oldalra, ok
int x = 0;								// d�nt�s el�re-h�tra 
int y = 0;								// oldalir�ny� d�l�s (0-90-ig: jobbra, 90-180-ig balra)

void setup() {
	//Serial.begin(9600);
	giroszkop.initialize();				// Giroszk�p ind�t�sa
	pinMode(gomb, INPUT);
	radio.begin();						// R�di� bekapcsol�sa
	radio.openWritingPipe(cim);			// csatorna megnyit�sa adatok k�ld�s�hez a k�zpontba
	radio.setPALevel(RF24_PA_MIN);		// R�di� t�rerej�nek minimumra �ll�t�sa
	GiroOffset();						// Giroszk�p null�z�sa
}


void loop() {
	giroszkop.getMotion6(&nyAx, &nyAy, &nyAz, &nyGx, &nyGy, &nyGz);			// ir�nyok kiolvas�sa
	int cAx = constrain(nyAx, XminNyers, XmaxNyers);						// �rt�kek beszor�t�sa a [0, 16000] intervallumba
	int cAy = constrain(nyAy, YminNyers, YmaxNyers);			
	csomag[0] = map(cAx, XminNyers, XmaxNyers, minSzog, maxSzog);			// limit�lt �rt�k konvert�l�sa a [0, 180] intervallumba
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

	radio.stopListening();													// ad�-m�dba kapcsolja a r�di�t
	radio.write(&csomag, sizeof(csomag));									// �tenet k�ld�se
}


void GiroOffset() {
	giroszkop.setXAccelOffset(-549);
	giroszkop.setYAccelOffset(-5894);
	giroszkop.setZAccelOffset(1684);
}

