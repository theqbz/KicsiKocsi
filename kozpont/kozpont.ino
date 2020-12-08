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
#include "C:/Program Files (x86)/Arduino/hardware/arduino/avr/cores/arduino/Arduino.h"
#include "C:/Program Files (x86)/Arduino/hardware/tools/avr/avr/include/stdlib.h"

// Arduino UNO Wifi R2 pin-kioszt�s
#define led 2
#define MotE 3					// motor Enable
#define MotC1 4					// motor t�p1
#define MotC2 5					// motor t�p2
#define RfCS 8					// az nRF24L01 modul "Chip Set" l�ba
#define RfCE 7					// az nRF24L01 modul "Chip Enable" l�ba
#define SrvPin 10				// a servo motor vez�rl�je

// nRF24L01 r�di� �lland�i
RF24 radio(RfCE, RfCS);					// R�di� l�trehoz�sa
const byte cim = 9654;					// a R�di� csatorn�j�nak c�me

// Servo
Servo kormany;							// Szerv� l�trehoz�sa

// egy�b glob�lis v�ltoz�k
byte kuldemeny[3];						// a t�vir�ny�t�b�l �rkez� adatok (el�re, oldalra, ok)

void setup() {
	pinMode(led, OUTPUT);
	pinMode(MotE, OUTPUT);
	pinMode(MotC1, OUTPUT);
	pinMode(MotC2, OUTPUT);
	Serial.begin(9600);
	radio.begin();						// R�di� bekapcsol�sa
	radio.openReadingPipe(0, cim);		// csatorna megnyit�sa adatok fogad�s�hoz a t�vir�ny�t�t�l
	radio.setPALevel(RF24_PA_MIN);		// R�di� t�rerej�nek minimumra �ll�t�sa
	kormany.attach(SrvPin);				// Szerv� vez�rl� csatlakoztat�sa
	kormany.write(90);					// korm�ny kiegyenes�t�se
	delay(2000);
}

void loop() {
	radio.startListening();								// vev�-m�dba kapcsolja a r�di�t

	if (radio.available())								// ha van fogadott adat (amit a t�vir�ny�t� k�ld�tt)
	{
		radio.read(&kuldemeny, sizeof(kuldemeny));				// adatok beolvas�sa
		if (kuldemeny[2] == 1)
		{
			int sebesseg = map(kuldemeny[0], 0, 255, -255, 255);
			if (sebesseg < -15)
			{
				digitalWrite(MotC1, LOW);
				digitalWrite(MotC2, HIGH);
				sebesseg = -sebesseg;
			}
			else if (sebesseg > 15)
			{
				digitalWrite(MotC1, HIGH);
				digitalWrite(MotC2, LOW);
			}
			else
			{
				sebesseg = 0;
			}
			KuldemenyKiiras(sebesseg);

			digitalWrite(led, HIGH);
			kormany.write(kuldemeny[1]);
			analogWrite(MotE, sebesseg);
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

// debug f�ggv�nyek

void KuldemenyKiiras(int sebesseg) {
	for (int i = 0; i < 3; i++)
	{
		Serial.print(kuldemeny[i]);
		Serial.print("\t");
	}
	Serial.println(sebesseg);
}