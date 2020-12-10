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
#define AKADALY 300				// ennél közelebb (kb. 4-5 cm ne menjen akadályokhoz)

// Arduino UNO Wifi R2 pin-kiosztás
#define HC1Echo 0				// Hátsó HC-SR04-4P Echo
#define HC1Trig 1				// Hátsó HC-SR04-4P Triger
#define HC2Echo 2				// Elsõ HC-SR04 Echo
#define HC2Trig 9				// Elsõ HC-SR04 Triger
#define MotC2 3					// H-bridge 2 (motor irány2)
#define MotC1 4					// H-bridge 7 (motor irány1)
#define MotE 5					// H-bridge 1 (motor ki-be)
#define Csipogo 6				// piezo hangszoro
#define RfCE 7					// nRF24L01 "Chip Enable"
#define RfCS 8					// nRF24L01 "Chip Set"
#define MOSI 11					// nRF24L01 MOSI (Az ICSP kommunikációhoz szükséges)
#define MISO 12					// nRF24L01 MISO (Az ICSP kommunikációhoz szükséges)
#define SCK 13					// nRF24L01 SCK (Az ICSP kommunikációhoz szükséges)
#define SrvPin 10				// Servo motor vezérlõje

// nRF24L01 rádió állandói
RF24 radio(RfCE, RfCS);			// Rádió létrehozása
const byte cim = 9654;			// a Rádió csatornájának címe

// Servo
Servo kormany;					// Szervó létrehozása

// egyéb globális változók
byte kuldemeny[3];				// a távirányítóból érkezõ adatok (elõre, oldalra, ok)
int kimaradas = 0;				// a távirányító elérhetetlenségének ideje
bool NemVoltKapcsolat = true;	// a távírányító nem volt csatlakozva
int sebesseg = 0;				// az autó sebessége
int TavEol;						// akadály távolsága elõl
int TavHatul;					// akadály távolsága hátrafelé
bool SerDebug = true;			// kiiírja-e a változókat a soros portra a ciklus végén




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

int ElsoLokator() {
	digitalWrite(HC2Trig, LOW);
	delayMicroseconds(2);
	digitalWrite(HC2Trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(HC2Trig, LOW);

	return int(pulseIn(HC2Echo, HIGH));
}

int HatsoLokator() {
	digitalWrite(HC1Trig, LOW);
	delayMicroseconds(2);
	digitalWrite(HC1Trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(HC1Trig, LOW);

	return int(pulseIn(HC1Echo, HIGH));
}

void SebessegIranyMeghatarozasa() {
	sebesseg = map(kuldemeny[0], 0, 255, -255, 255);
	if (sebesseg < -LIMIT)
	{
		digitalWrite(MotC1, LOW);
		digitalWrite(MotC2, HIGH);
		if (HatsoLokator() < AKADALY)
		{
			sebesseg = 0;
		}
		else
		{
			sebesseg = -sebesseg;
		}
	}
	else if (sebesseg > LIMIT)
	{
		digitalWrite(MotC1, HIGH);
		digitalWrite(MotC2, LOW);
		if (ElsoLokator() < AKADALY)
		{
			sebesseg = 0;
		}
	}
	else
	{
		sebesseg = 0;
	}
}

void BejovoAdatokFeldolgozasa() {
	if (NemVoltKapcsolat)						// jelzés, ha most lett újra online a távirányító
	{
		LettKapcsolat();
	}
	radio.read(&kuldemeny, sizeof(kuldemeny));	// adatok beolvasása
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
	radio.startListening();				// vevõ-módba kapcsolja a rádiót
	if (radio.available())				// ha van fogadott adat (amit a távirányító küldött)
	{
		BejovoAdatokFeldolgozasa();
	}
	else								// ha nincs adat, minden bizonnyal megszakadt a kapcsolat
	{
		kimaradas++;
	}

	if (kimaradas == TIMEOUT)			// Ha megszakadt a kapcsolat, akkor le kell állni
	{
		NicsKapcsolat();
	}
}

void Mozgas() {
	kormany.write(kuldemeny[1]);
	analogWrite(MotE, sebesseg);
}


void setup() {
	pinMode(MotE, OUTPUT);
	pinMode(MotC1, OUTPUT);
	pinMode(MotC2, OUTPUT);
	pinMode(HC1Trig, OUTPUT);
	pinMode(HC1Echo, INPUT);
	pinMode(HC2Trig, OUTPUT);
	pinMode(HC2Echo, INPUT);
	Serial.begin(9600);
	radio.begin();						// Rádió bekapcsolása
	radio.openReadingPipe(0, cim);		// csatorna nyitása adatok fogadásához a távirányítótól
	radio.setPALevel(RF24_PA_MIN);		// Rádió térerejének minimumra állítása
	kormany.attach(SrvPin);				// Szervó vezérlõ csatlakoztatása
	kormany.write(90);					// kormány kiegyenesítése
	delay(2000);
}

void loop() {
	TaviranyitoAdatainakOlvasasa();		//Távirányító adatainak feldolgozása
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
			Serial.print("kuld[");
			Serial.print(i);
			Serial.print("]: ");
			Serial.print(kuldemeny[i]);
			Serial.print("\t");
		}
		Serial.print("kimarad: ");
		Serial.print(kimaradas);
		Serial.print("\tseb: ");
		Serial.print(sebesseg);
		Serial.print("\telol: ");
		Serial.print(TavEol);
		Serial.print("\thatul: ");
		Serial.println(TavHatul);
	}
}