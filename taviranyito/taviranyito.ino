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
int16_t ax, ay, az;						// gyorsulás az adott tengelyen
int16_t gx, gy, gz;						// elfordulás az adott tengelyen
//#define OUTPUT_READABLE_ACCELGYRO

// egyéb globális változók
long uzenet = 0L;						// Be/Ki és az irányok egy 7 jegyû egész számban
int tomb[2];

void setup() {
	Serial.begin(9600);
	giroszkop.initialize();				// Giroszkóp indítása
	pinMode(gomb, INPUT);
	radio.begin();						// Rádió bekapcsolása
	radio.openWritingPipe(cim);			// csatorna megnyitása adatok küldéséhez a központba
	radio.setPALevel(RF24_PA_MIN);		// Rádió térerejének minimumra állítása
	GiroOffset();						// Giroszkóp nullázása
}


void loop() {
	radio.stopListening();							// adó-módba kapcsolja a rádiót

	giroszkop.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);		// irányok kiolvasása
	GyroPrint();

	
	if (digitalRead(gomb))
	{
		uzenet = 1L;
		tomb[0] = 1;
		tomb[1] = 12;
		radio.write(tomb, sizeof(tomb));			// ütenet küldése
//		Serial.println("megnyomva: BE");
	}
	else
	{
		uzenet = 0L;
		tomb[0] = 2;
		tomb[1] = 22;
		radio.write(tomb, sizeof(tomb));			// ütenet küldése
//		Serial.println("nincs megnyomva: KI");
	}
	delay(5);
}


void GyroPrint() {
	Serial.print(ax);
	Serial.print("\t");
	Serial.print(ay);
	Serial.print("\t");
	Serial.println(az); 
}

void GiroOffset() {
	giroszkop.setXAccelOffset(-549);
	giroszkop.setYAccelOffset(-5894);
	giroszkop.setZAccelOffset(1684);
}


int ErtekKonverzio(int16_t nyX, int16_t nyY, int16_t nyZ) {

}