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

#define LIMIT 100				// a limit alatti értéknél nem indul a motor
#define TIMEOUT 100				// riasztás, ha ennyi cikluson át nem üzen a távirányító

// Arduino UNO Wifi R2 pin-kiosztás
#define LED 2
#define MotE 3					// motor Enable
#define MotC1 4					// motor táp1
#define MotC2 5					// motor táp2
#define Csipogo 6				// piezo hangszoro
#define RfCS 8					// az nRF24L01 modul "Chip Set" lába
#define RfCE 7					// az nRF24L01 modul "Chip Enable" lába
#define SrvPin 10				// a servo motor vezérlõje

// nRF24L01 rádió állandói
RF24 radio(RfCE, RfCS);			// Rádió létrehozása
const byte cim = 9654;			// a Rádió csatornájának címe

// Servo
Servo kormany;					// Szervó létrehozása

// egyéb globális változók
byte kuldemeny[3];				// a távirányítóból érkezõ adatok (elõre, oldalra, ok)
int kimaradas = 0;				// a távirányító elérhetetlenségének ideje
bool kapcsolat = true;			// a távírányító csatlakozva

void setup() {
	pinMode(LED, OUTPUT);
	pinMode(MotE, OUTPUT);
	pinMode(MotC1, OUTPUT);
	pinMode(MotC2, OUTPUT);
	Serial.begin(9600);
	radio.begin();					// Rádió bekapcsolása
	radio.openReadingPipe(0, cim);	// csatorna nyitása adatok fogadásához a távirányítótól
	radio.setPALevel(RF24_PA_MIN);	// Rádió térerejének minimumra állítása
	kormany.attach(SrvPin);			// Szervó vezérlõ csatlakoztatása
	kormany.write(90);				// kormány kiegyenesítése
	delay(2000);
}

void loop() {
	radio.startListening();				// vevõ-módba kapcsolja a rádiót

	if (radio.available())				// ha van fogadott adat (amit a távirányító küldött)
	{
		if (!kapcsolat)
		{
			tone(Csipogo, 880, 40);
			delay(250);
			tone(Csipogo, 1760, 40);
			kapcsolat = true;
		}
		radio.read(&kuldemeny, sizeof(kuldemeny));				// adatok beolvasása
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