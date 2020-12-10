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
#define HCEcho 2				// HC-SR04 Echo
#define HCTrig 9				// HC-SR04 Triger
#define MotC2 3					// H-bridge 2 (motor ir�ny2)
#define MotC1 4					// H-bridge 7 (motor ir�ny1)
#define MotE 5					// H-bridge 1 (motor ki-be)
#define Csipogo 6				// piezo hangszoro
#define RfCE 7					// nRF24L01 "Chip Enable"
#define RfCS 8					// nRF24L01 "Chip Set"
#define MOSI 11					// nRF24L01 MOSI (Az ICSP kommunik�ci�hoz sz�ks�ges)
#define MISO 12					// nRF24L01 MISO (Az ICSP kommunik�ci�hoz sz�ks�ges)
#define SCK 13					// nRF24L01 SCK (Az ICSP kommunik�ci�hoz sz�ks�ges)
#define SrvPin 10				// Servo motor vez�rl�je

// nRF24L01 r�di� �lland�i
RF24 radio(RfCE, RfCS);			// R�di� l�trehoz�sa
const byte cim = 9654;			// a R�di� csatorn�j�nak c�me

// Servo
Servo kormany;					// Szerv� l�trehoz�sa

// egy�b glob�lis v�ltoz�k
byte kuldemeny[3];				// a t�vir�ny�t�b�l �rkez� adatok (el�re, oldalra, ok)
int kimaradas = 0;				// a t�vir�ny�t� el�rhetetlens�g�nek ideje
bool NemVoltKapcsolat = true;	// a t�v�r�ny�t� nem volt csatlakozva
int sebesseg = 0;				// az aut� sebess�ge
long ido;						// a visszaver�d�sig eltelt id� a lok�rotban
int tavolsag;					// fal t�vols�ga
bool SerDebug = true;			// kii�rja-e a v�ltoz�kat a soros portra a ciklus v�g�n




void LettKapcsolat() {
	tone(Csipogo, 880, 40);
	delay(250);
	tone(Csipogo, 1760, 40);
	NemVoltKapcsolat = false;
}

void NicsKapcsolat() {
	tone(Csipogo, 1000, 40);
	delay(100);
	tone(Csipogo, 880, 20);
	delay(100);
	tone(Csipogo, 880, 20);
	kimaradas = 0;
	NemVoltKapcsolat = true;
	sebesseg = 0;
}

void SebessegIranyMeghatarozasa() {
	sebesseg = map(kuldemeny[0], 0, 255, -255, 255);
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
}

void BejovoAdatokFeldolgozasa() {
	if (NemVoltKapcsolat)						// jelz�s, ha most lett �jra online a t�vir�ny�t�
	{
		LettKapcsolat();
	}
	radio.read(&kuldemeny, sizeof(kuldemeny));	// adatok beolvas�sa
	kimaradas = 0;
	if (kuldemeny[2] == 1)
	{
		SebessegIranyMeghatarozasa();
	}
	else
	{
		sebesseg = 0;
	}
}

void TaviranyitoAdatainakOlvasasa() {
	radio.startListening();				// vev�-m�dba kapcsolja a r�di�t
	if (radio.available())				// ha van fogadott adat (amit a t�vir�ny�t� k�ld�tt)
	{
		BejovoAdatokFeldolgozasa();
	}
	else								// ha nincs adat, minden bizonnyal megszakadt a kapcsolat
	{
		kimaradas++;
		Serial.print("valami nem oke");
		Serial.print("\t");
		Serial.println(kimaradas);
	}

	if (kimaradas == TIMEOUT)			// Ha megszakadt a kapcsolat, akkor le kell �llni
	{
		NicsKapcsolat();
	}
}

void Lokator() {
	digitalWrite(HCTrig, LOW);
	delayMicroseconds(2);
	digitalWrite(HCTrig, HIGH);
	delayMicroseconds(10);
	digitalWrite(HCTrig, LOW);

	ido = pulseIn(HCEcho, HIGH);
	tavolsag = ido * 0.034 / 2;
}

void Mozgas() {
	kormany.write(kuldemeny[1]);
	analogWrite(MotE, sebesseg);
}


void setup() {
	pinMode(MotE, OUTPUT);
	pinMode(MotC1, OUTPUT);
	pinMode(MotC2, OUTPUT);
	pinMode(HCTrig, OUTPUT);
	pinMode(HCEcho, INPUT);

	Serial.begin(9600);
	radio.begin();						// R�di� bekapcsol�sa
	radio.openReadingPipe(0, cim);		// csatorna nyit�sa adatok fogad�s�hoz a t�vir�ny�t�t�l
	radio.setPALevel(RF24_PA_MIN);		// R�di� t�rerej�nek minimumra �ll�t�sa
	kormany.attach(SrvPin);				// Szerv� vez�rl� csatlakoztat�sa
	kormany.write(90);					// korm�ny kiegyenes�t�se
	delay(2000);
}

void loop() {
	TaviranyitoAdatainakOlvasasa();		//T�vir�ny�t� adatainak feldolgoz�sa
	Lokator();
	Mozgas();							// Mehet a kocsi

	DEBUGmonitor();						// debug
	delay(20);
}





// debug

void DEBUGmonitor() {
	if (Serial.available())
	{
		if (Serial.readString()=="serialstop")
		{
			SerDebug = false;
//			Serial.println("monitor kikapcsolva");
		}
		else if(Serial.readString()=="serialstart")
		{
			SerDebug = true;
		}
	}
	if (SerDebug)
	{
		for (int i = 0; i < 3; i++)
		{
			Serial.print("kuldemeny[");
			Serial.print(i);
			Serial.print("]: ");
			Serial.print(kuldemeny[i]);
			Serial.print("\t");
		}
		Serial.print("\tkimaradas: ");
		Serial.print(kimaradas);
		Serial.print("\tsebesseg: ");
		Serial.print(sebesseg);
		Serial.print("\ttavolsag: ");
		Serial.print(tavolsag);
		Serial.println(" cm");
	}
}