/*
 Name:		KICSIKOCSI TÁVIRÁNYÍTÓ
		Arduino Nano
		
*/


#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <MPU6050.h>


// Arduino Nano pin-kiosztás
#define gomb 5
#define led 4
#define RfCS 7					// nRF24L01 "Chip Set"
#define RfCE 8					// nRF24L01 "Chip Enable"
#define MOSI 11					// nRF24L01 MOSI (Az ICSP kommunikációhoz szükséges)
#define MISO 12					// nRF24L01 MISO (Az ICSP kommunikációhoz szükséges)
#define SCK 13					// nRF24L01 SCK (Az ICSP kommunikációhoz szükséges)
#define MINT 10					// MPU6050 INT
#define MSDA A4					// MPU6050 SDA
#define MSCL A5					// MPU6050 SCL

#define XminNyers -16000		// a nyers adatok alsó határa levágáshoz
#define XmaxNyers 16000			// a nyers adatok felső határa levágáshoz
#define YminNyers -16000		// a nyers adatok alsó határa levágáshoz
#define YmaxNyers 16000			// a nyers adatok felső határa levágáshoz
#define minSeb 0				// a konverzió alsó határa sebességhez
#define maxSeb 255				// a konverzió felső határa sebességhez
#define minSzog 1				// a konverzió alsó határa kormány-szöghöz
#define maxSzog 179				// a konverzió felső határa kormány-szöghöz


// nRF24L01 rádió állandói
RF24 radio(RfCE, RfCS);						// Rádió
const byte cim = 96;						// a Rádió csatornájának címe

// MPU6050 giroszkóp állandói
MPU6050 giroszkop;							// Giroszkóp
int16_t nyAx, nyAy, nyAz, nyGx, nyGy, nyGz;	// nyers adatok a giroszkópról

// egyéb globális változó
byte csomag[3] = { 0,0,0 };					// Egy adatcsomag: előre, oldalra, ok


void GiroOffset()
{
	giroszkop.setXAccelOffset(-549);
	giroszkop.setYAccelOffset(-5894);
	giroszkop.setZAccelOffset(1684);
	giroszkop.setXGyroOffset(-165);
	giroszkop.setYGyroOffset(228);
	giroszkop.setZGyroOffset(-20);
}


void setup()
{
	Serial.begin(9600);
	giroszkop.initialize();				// Giroszkóp indítása
	pinMode(led, OUTPUT);
	pinMode(gomb, INPUT);
	radio.begin();						// Rádió bekapcsolása
	radio.openWritingPipe(cim);			// csatorna megnyitása adatok küldéséhez a központba
	radio.setPALevel(RF24_PA_MIN);		// Rádió térerejének minimumra állítása
	GiroOffset();						// Giroszkóp nullázása
	digitalWrite(led, HIGH);
}

void loop()
{
	giroszkop.getMotion6(&nyAx, &nyAy, &nyAz, &nyGx, &nyGy, &nyGz);			// irányok kiolvasása
	int cAx = constrain(nyAx, XminNyers, XmaxNyers);						// értékek beszorítása a [0, 16000] intervallumba
	int cAy = constrain(nyAy, YminNyers, YmaxNyers);
	int cAz = constrain(nyAz, YminNyers, YmaxNyers);
	int cGx = constrain(nyGx, XminNyers, XmaxNyers);
	int cGy = constrain(nyGy, YminNyers, YmaxNyers);
	int cGz = constrain(nyGz, YminNyers, YmaxNyers);

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
	radio.stopListening();													// adó-módba kapcsolja a rádiót
	radio.write(&csomag, sizeof(csomag));									// ütenet küldése
//	DEBUG(cAx, cAy, cAz, cGx, cGy, cGz);
}




////debug
//void DEBUG(int x, int y, int z, int gx, int gy, int gz) {
//	Serial.print("AX: ");
//	Serial.print(x);
//	Serial.print("\tAY: ");
//	Serial.print(y);
//	Serial.print("\tAZ: ");
//	Serial.print(z);
//	Serial.print("\tGX: ");
//	Serial.print(gx);
//	Serial.print("\tGY: ");
//	Serial.print(gy);
//	Serial.print("\tGZ: ");
//	Serial.println(gz);
//}
