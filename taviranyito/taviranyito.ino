/*
KICSIKOCSI T�VIR�NY�T�

 Name:		kicsikocsi.ino
 Created:	12/5/2020 6:14:38 PM
 Author:	qbz
*/



#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <MPU6050.h>

#define XminNyers -16000		// a nyers adatok als� hat�ra lev�g�shoz
#define XmaxNyers 16000			// a nyers adatok fels� hat�ra lev�g�shoz
#define YminNyers -16000		// a nyers adatok als� hat�ra lev�g�shoz
#define YmaxNyers 16000			// a nyers adatok fels� hat�ra lev�g�shoz
#define minSeb 0				// a konverzi� als� hat�ra sebess�ghez
#define maxSeb 255				// a konverzi� fels� hat�ra sebess�ghez
#define minSzog 1				// a konverzi� als� hat�ra korm�ny-sz�gh�z
#define maxSzog 179				// a konverzi� fels� hat�ra korm�ny-sz�gh�z

// Arduino Nano pin-kioszt�s
#define gomb 5
#define led 4
#define RfCS 7					// nRF24L01 "Chip Set"
#define RfCE 8					// nRF24L01 "Chip Enable"
#define MOSI 11					// nRF24L01 MOSI (Az ICSP kommunik�ci�hoz sz�ks�ges)
#define MISO 12					// nRF24L01 MISO (Az ICSP kommunik�ci�hoz sz�ks�ges)
#define SCK 13					// nRF24L01 SCK (Az ICSP kommunik�ci�hoz sz�ks�ges)
#define MINT 10					// MPU6050 INT
#define MSDA A4					// MPU6050 SDA
#define MSCL A5					// MPU6050 SCL



// nRF24L01 r�di� �lland�i
RF24 radio(RfCE, RfCS);						// R�di�
const byte cim = 96;						// a R�di� csatorn�j�nak c�me

// MPU6050 giroszk�p �lland�i
MPU6050 giroszkop;							// Giroszk�p
int16_t nyAx, nyAy, nyAz, nyGx, nyGy, nyGz;	// nyers adatok a giroszk�pr�l

// egy�b glob�lis v�ltoz�
byte csomag[3] = { 0,0,0 };					// Egy adatcsomag: el�re, oldalra, ok





void GiroOffset() {
	giroszkop.setXAccelOffset(-549);
	giroszkop.setYAccelOffset(-5894);
	giroszkop.setZAccelOffset(1684);
	giroszkop.setXGyroOffset(-165);
	giroszkop.setYGyroOffset(228);
	giroszkop.setZGyroOffset(-20);
}




void setup() {
	Serial.begin(9600);
	giroszkop.initialize();				// Giroszk�p ind�t�sa
	pinMode(led, OUTPUT);
	pinMode(gomb, INPUT);
	radio.begin();						// R�di� bekapcsol�sa
	radio.openWritingPipe(cim);			// csatorna megnyit�sa adatok k�ld�s�hez a k�zpontba
	radio.setPALevel(RF24_PA_MIN);		// R�di� t�rerej�nek minimumra �ll�t�sa
	GiroOffset();						// Giroszk�p null�z�sa
	digitalWrite(led, HIGH);
}

void loop() {
	giroszkop.getMotion6(&nyAx, &nyAy, &nyAz, &nyGx, &nyGy, &nyGz);			// ir�nyok kiolvas�sa
	int cAx = constrain(nyAx, XminNyers, XmaxNyers);						// �rt�kek beszor�t�sa a [0, 16000] intervallumba
	int cAy = constrain(nyAy, YminNyers, YmaxNyers);
	int cAz = constrain(nyAz, YminNyers, YmaxNyers);
	int cGx = constrain(nyGx, XminNyers, XmaxNyers);
	int cGy = constrain(nyGy, YminNyers, YmaxNyers);
	int cGz = constrain(nyGz, YminNyers, YmaxNyers);

	csomag[0] = map(cAx, XminNyers, XmaxNyers, minSeb, maxSeb);				// sebess�g �rt�k konvert�l�sa a [0, 255] intervallumba
	csomag[1] = map(cAy, YminNyers, YmaxNyers, minSzog, maxSzog);			// ir�ny konvert�l�sa a [0,180] intervallumba
	if (digitalRead(gomb))
	{
		csomag[2] = 1;
	}
	else
	{
		csomag[2] = 1;
	}
	radio.stopListening();													// ad�-m�dba kapcsolja a r�di�t
	radio.write(&csomag, sizeof(csomag));									// �tenet k�ld�se
	DEBUG(cAx, cAy, cAz, cGx, cGy, cGz);
}




//debug
void DEBUG(int x, int y, int z, int gx, int gy, int gz) {
	Serial.print("AX: ");
	Serial.print(x);
	Serial.print("\tAY: ");
	Serial.print(y);
	Serial.print("\tAZ: ");
	Serial.print(z);
	Serial.print("\tGX: ");
	Serial.print(gx);
	Serial.print("\tGY: ");
	Serial.print(gy);
	Serial.print("\tGZ: ");
	Serial.println(gz);
}