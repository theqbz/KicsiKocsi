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

#define LIMIT 100				// a limit alatti �rt�kn�l nem indul a motor
#define TIMEOUT 100				// riaszt�s, ha ennyi cikluson �t nem �zen a t�vir�ny�t�

// Arduino UNO Wifi R2 pin-kioszt�s
#define LED 2
#define MotE 3					// motor Enable
#define MotC1 4					// motor t�p1
#define MotC2 5					// motor t�p2
#define Csipogo 6				// piezo hangszoro
#define RfCS 8					// az nRF24L01 modul "Chip Set" l�ba
#define RfCE 7					// az nRF24L01 modul "Chip Enable" l�ba
#define SrvPin 10				// a servo motor vez�rl�je

// nRF24L01 r�di� �lland�i
RF24 radio(RfCE, RfCS);			// R�di� l�trehoz�sa
const byte cim = 9654;			// a R�di� csatorn�j�nak c�me

// Servo
Servo kormany;					// Szerv� l�trehoz�sa

// egy�b glob�lis v�ltoz�k
byte kuldemeny[3];				// a t�vir�ny�t�b�l �rkez� adatok (el�re, oldalra, ok)
int kimaradas = 0;				// a t�vir�ny�t� el�rhetetlens�g�nek ideje
bool kapcsolat = true;			// a t�v�r�ny�t� csatlakozva

void setup() {
	pinMode(LED, OUTPUT);
	pinMode(MotE, OUTPUT);
	pinMode(MotC1, OUTPUT);
	pinMode(MotC2, OUTPUT);
	Serial.begin(9600);
	radio.begin();					// R�di� bekapcsol�sa
	radio.openReadingPipe(0, cim);	// csatorna nyit�sa adatok fogad�s�hoz a t�vir�ny�t�t�l
	radio.setPALevel(RF24_PA_MIN);	// R�di� t�rerej�nek minimumra �ll�t�sa
	kormany.attach(SrvPin);			// Szerv� vez�rl� csatlakoztat�sa
	kormany.write(90);				// korm�ny kiegyenes�t�se
	delay(2000);
}

void loop() {
	radio.startListening();				// vev�-m�dba kapcsolja a r�di�t

	if (radio.available())				// ha van fogadott adat (amit a t�vir�ny�t� k�ld�tt)
	{
		if (!kapcsolat)
		{
			tone(Csipogo, 880, 40);
			delay(250);
			tone(Csipogo, 1760, 40);
			kapcsolat = true;
		}
		radio.read(&kuldemeny, sizeof(kuldemeny));				// adatok beolvas�sa
		kimaradas = 0;
		if (kuldemeny[2] == 1)
		{
			int sebesseg = map(kuldemeny[0], 0, 255, -255, 255);
			if (sebesseg < -LIMIT)
			{
				digitalWrite(MotC1, LOW);
				digitalWrite(MotC2, HIGH);
				sebesseg = -sebesseg;
			}
			else if (sebesseg > LIMIT)
			{
				digitalWrite(MotC1, HIGH);
				digitalWrite(MotC2, LOW);
			}
			else
			{
				sebesseg = 0;
			}

			KuldemenyKiiras(sebesseg);

			digitalWrite(LED, HIGH);
			kormany.write(kuldemeny[1]);
			analogWrite(MotE, sebesseg);
		}
		else
		{
			digitalWrite(LED, LOW);
		}
	}
	else
	{
		digitalWrite(LED, LOW);
		kimaradas++;
		kapcsolat = false;
		Serial.print("valami nem oke");
		Serial.print("\t");
		Serial.println(kimaradas);
	}
	if (kimaradas == TIMEOUT)
	{
		tone(Csipogo, 1000, 40);
		delay(100);
		tone(Csipogo, 880, 20);
		delay(100);
		tone(Csipogo, 880, 20);
		kimaradas = 0;
	}
	delay(20);
}

// debug

void KuldemenyKiiras(int sebesseg) {
	for (int i = 0; i < 3; i++)
	{
		Serial.print(kuldemeny[i]);
		Serial.print("\t");
	}
	Serial.print(sebesseg);
	Serial.print("\t");
	Serial.println(kimaradas);
}