/*
KICSIKOCSI KÖPONTI EGYSÉG

 Name:		kozpont.ino
 Created:	12/5/2020 8:17:27 PM
 Author:	qbz
*/


#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

// Arduino UNO Wifi R2 pin-kiosztás
#define led 2
#define RfCS 8					// az nRF24L01 modul "Chip Set" lába
#define RfCE 7					// az nRF24L01 modul "Chip Enable" lába
#define SrvPin 10				// a servo motor vezérlõje

// nRF24L01 rádió állandói
RF24 radio(RfCE, RfCS);					// Rádió létrehozása
const byte cim = 9654;					// a Rádió csatornájának címe

// Servo
Servo kormany;							// Szervó létrehozása

// egyéb globális változók
byte kuldemeny[3];						// a távirányítóból érkezõ adatok (elõre, oldalra, ok)

void setup() {
	pinMode(led, OUTPUT);
//	Serial.begin(9600);
	radio.begin();						// Rádió bekapcsolása
	radio.openReadingPipe(0, cim);		// csatorna megnyitása adatok fogadásához a távirányítótól
	radio.setPALevel(RF24_PA_MIN);		// Rádió térerejének minimumra állítása
	kormany.attach(SrvPin);				// Szervó vezérlõ csatlakoztatása
	kormany.write(90);					// kormány kiegyenesítése
	delay(2000);
}

void loop() {
	radio.startListening();								// vevõ-módba kapcsolja a rádiót

	if (radio.available())								// ha van fogadott adat (amit a távirányító küldött)
	{
		radio.read(&kuldemeny, sizeof(kuldemeny));				// adatok beolvasása
		if (kuldemeny[2] == 1)
		{
			int kanyar = map(kuldemeny[1], 0, 180, 1, 179);
			kormany.write(kanyar);
			digitalWrite(led, HIGH);
		}
		else
		{
			digitalWrite(led, LOW);
		}
	}
	else
	{
		Serial.println("valami nem oke");
	}
	delay(20);
}


