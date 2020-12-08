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
#define kormany 10				// a servo motor vezérlõje

// nRF24L01 rádió állandói
RF24 radio(RfCE, RfCS);					// Rádió létrehozása
const byte cim = 9654;					// a Rádió csatornájának címe

// egyéb globális változók
byte kuldemeny[3];
int ftomb[2];

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	radio.begin();						// Rádió bekapcsolása
	radio.openReadingPipe(0, cim);		// csatorna megnyitása adatok fogadásához a távirányítótól
	radio.setPALevel(RF24_PA_MIN);		// Rádió térerejének minimumra állítása
}

void loop() {
	radio.startListening();								// vevõ-módba kapcsolja a rádiót

	if (radio.available())								// ha van fogadott adat (amit a távirányító küldött)
	{
		radio.read(&kuldemeny, sizeof(kuldemeny));				// adatok beolvasása
		Serial.print(kuldemeny[2]);
		if (kuldemeny[2] == 1)
		{
			Serial.print(" bekapcs ag\t");
			Serial.print(kuldemeny[0]);
			Serial.print("\t");
			Serial.println(kuldemeny[1]);
			digitalWrite(led, HIGH);
		}
		else
		{
			Serial.print(" kikapcs ag\t");
			Serial.print(kuldemeny[0]);
			Serial.print("\t");
			Serial.println(kuldemeny[1]);
			digitalWrite(led, LOW);
		}
	}
	else
	{
		Serial.println("valami nem oke");
	}
	delay(20);
}


