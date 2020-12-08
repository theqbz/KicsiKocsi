/*
KICSIKOCSI K�PONTI EGYS�G

 Name:		kozpont.ino
 Created:	12/5/2020 8:17:27 PM
 Author:	qbz
*/


#include <Servo.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

// Arduino UNO Wifi R2 pin-kioszt�s
#define led 2
#define RfCS 8					// az nRF24L01 modul "Chip Set" l�ba
#define RfCE 7					// az nRF24L01 modul "Chip Enable" l�ba
#define kormany 10				// a servo motor vez�rl�je

// nRF24L01 r�di� �lland�i
RF24 radio(RfCE, RfCS);					// R�di� l�trehoz�sa
const byte cim = 9654;					// a R�di� csatorn�j�nak c�me

// egy�b glob�lis v�ltoz�k
byte kuldemeny[3];
int ftomb[2];

void setup() {
	pinMode(led, OUTPUT);
	Serial.begin(9600);
	radio.begin();						// R�di� bekapcsol�sa
	radio.openReadingPipe(0, cim);		// csatorna megnyit�sa adatok fogad�s�hoz a t�vir�ny�t�t�l
	radio.setPALevel(RF24_PA_MIN);		// R�di� t�rerej�nek minimumra �ll�t�sa
}

void loop() {
	radio.startListening();								// vev�-m�dba kapcsolja a r�di�t

	if (radio.available())								// ha van fogadott adat (amit a t�vir�ny�t� k�ld�tt)
	{
		radio.read(&kuldemeny, sizeof(kuldemeny));				// adatok beolvas�sa
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


