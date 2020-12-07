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
long uzenet = 0L;						// Be/Ki és az irányok egy 7 jegyû egész számban
int tomb[2];
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
	x = map(cAx, XminNyers, XmaxNyers, minSzog, maxSzog);					// limitált érték konvertálása a [0, 180] intervallumba
	y = map(cAy, YminNyers, YmaxNyers, minSzog, maxSzog);
	delay(5);

	radio.stopListening();													// adó-módba kapcsolja a rádiót


	//GyroPrint();

	
//	if (digitalRead(gomb))
//	{
//		uzenet = 1L;
//		tomb[0] = 1;
//		tomb[1] = 12;
//		radio.write(tomb, sizeof(tomb));			// ütenet küldése
////		Serial.println("megnyomva: BE");
//	}
//	else
//	{
//		uzenet = 0L;
//		tomb[0] = 2;
//		tomb[1] = 22;
//		radio.write(tomb, sizeof(tomb));			// ütenet küldése
////		Serial.println("nincs megnyomva: KI");
//	}
	
}


//void GyroPrint() {
//	//Serial.print("X=");
//	//Serial.print(nyAx);
//	Serial.print("\tnyY=");
//	Serial.print(nyAy);
//	//Serial.print("\tZ=");
//	//Serial.println(nyAz);
//	Serial.print("\tY=");
//	Serial.println(y); 
//}

void GiroOffset() {
	giroszkop.setXAccelOffset(-549);
	giroszkop.setYAccelOffset(-5894);
	giroszkop.setZAccelOffset(1684);
}


int ErtekKonverzio(int16_t nyX, int16_t nyY, int16_t nyZ) {

}