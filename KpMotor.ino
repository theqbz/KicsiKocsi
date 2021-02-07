/*
		MOTOR (KICSIKOCSI KOZPONTI EGYSEG)

		kesztitette: Borsodi Zoltan
			     github.com/theqbz

	Arduino Nano:

	A4  -  SDA (I2C kommunikacio)
	A5  -  SCL (I2C kommunikacio)
	D3  -  L293 'Input1' (elore forog a motor)
	D4  -  L293 'Input2' (hatra forog a motor)
	D5  -  A szervo motor vezerloje
	D6  -  L293 'Enable' (motor bekapcsolasa)
	D7  -  nRF24L01 'Chip Enable'
	D8  -  nRF24L01 'Chip Set'
	D9  -  piezzo hangkerlto /csipogo/
	D11 -  nRF24L01 MOSI (Az ICSP kommunikaciohoz szukseges)
	D12 -  nRF24L01 MISO (Az ICSP kommunikaciohoz szukseges)
	D13 -  nRF24L01 SCK (Az ICSP kommunikaciohoz szukseges)


*/

#include <SPI.h>
#include <RF24_config.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
#include <Wire.h>
#include <Servo.h>

// Arduino Nano pin kiosztas
#define MOTOR_ELORE 3							
#define MOTOR_HATRA 4							
#define SERVO 5								
#define MOTOR_ENABLE 6							
#define RF_CE 7								
#define RF_CS 8								
#define PIEZZO 9							
#define RF_MOSI 11							
#define RF_MISO 12							
#define RF_SCK 13							

// egyeb allandok
#define TIMEOUT 100							// ennyi ciklus utan riaszt, ha offline a taviranyito
#define AKADALY 20							// ennel kozelebb (centimeterben)
#define MIN_SEBESSEG 100						// limit alatti erteknel nem indul a motor


RF24 radio(RF_CE, RF_CS);						// Radio letrehozasa
Servo kormany;								// Kormany letrehozasa

byte kontrollerSebesseg;						// A taviranyito altal kuldott sebesseg adat
byte kontrollerKormany;							// A taviranyito altal kuldott kormanyzas adat
bool kontrollerGomb;							// A taviranyito gombjanak allapota
int kontrollerOfflineTimer;						// A taviranyito offline allapotanak kesleltetese
bool kontrollerOnline;							// A taviranyito online allapota
byte elsoLokator;							// Az elso lokator altal mert tavolsag
byte hatsoLokator;							// A hatso lokator altal mert tavolsag
bool akadalyok[2];							// Marker az akadaly jelzesehez

void mozgas()
{
	lokatorOlvasasa();
	kontrollerOlvasasa();
	kormany.write(kontrollerKormany);
	analogWrite(MOTOR_ENABLE, sebessegSzamitasa());
}

void lokatorOlvasasa()
{
	Wire.requestFrom(0x02, 2);
	if (Wire.available()) {
		elsoLokator = Wire.read();
		hatsoLokator = Wire.read();
	}
}

void kontrollerOlvasasa()
{
	radio.startListening();
	if (radio.available()) radioAdatokFeldolgozasa();
	else kontrollerOfflineTimer++;
	if (kontrollerOfflineTimer == TIMEOUT) nincsKapcsolat();
}

void radioAdatokFeldolgozasa()
{
	if (!kontrollerOnline) lettKapcsolat();
	byte csomag[3];
	radio.read(&csomag, sizeof(csomag));
	kontrollerOfflineTimer = 0;
	kontrollerSebesseg = map(csomag[0], 0, 255, 0, 255);
	kontrollerKormany = map(csomag[1], 1, 179, 179, 1);
	kontrollerGomb = csomag[2];
}

void nincsKapcsolat()
{
	tone(PIEZZO, 1000, 40);
	delay(100);
	tone(PIEZZO, 880, 20);
	delay(100);
	tone(PIEZZO, 880, 20);
	kontrollerOnline = false;
	kontrollerOfflineTimer = 0;
}

void lettKapcsolat()
{
	tone(PIEZZO, 880, 40);
	delay(250);
	tone(PIEZZO, 1760, 40);
	kontrollerOnline = true;
}

void akadalyJelzes(byte irany)
{
	if (!akadalyok[irany]) {
		tone(PIEZZO, 440, 20);
		delay(100);
		tone(PIEZZO, 440, 20);
		akadalyok[irany]=true;
	}
}

int sebessegSzamitasa()
{
	if (kontrollerGomb == 1 && kontrollerOnline) {
		int sebesseg = map(kontrollerSebesseg, 0, 255, -255, 255);

		// ha a sebesseg alapjan hatrament van es a sebesseg (abszoluterteke) nagyobb a minimumnal
		if (sebesseg < -MIN_SEBESSEG) {
			digitalWrite(MOTOR_ELORE, LOW);
			digitalWrite(MOTOR_HATRA, HIGH);

			// ha a hatso lokotor alapjan kozel egy akadaly
			if (hatsoLokator < AKADALY) {
				sebesseg = 0;
				akadalyJelzes(0);
			}

			// ha nincs akadaly, akkor a negativ sebessegertek abszoluterteke
			else {
				sebesseg = -sebesseg;
				akadalyok[0]=false;
			}
		}

		// ha eloremenet van es a sebesseg meghaladja a minimumiot
		else if (sebesseg > MIN_SEBESSEG) {
			digitalWrite(MOTOR_ELORE, HIGH);
			digitalWrite(MOTOR_HATRA, LOW);

			// ha az elso lokotor alapjan kozel egy akadaly
			if (elsoLokator < AKADALY) {
				sebesseg = 0;
				akadalyJelzes(1);
			}

		// ha nincs akadaly, akkor nincs kulon teendo a sebesseggel
		akadalyok[1]=false;
		}

		// amennyiben a sebesseg nem haladja meg a minimalis merteket, nincs mozgas
		else {
			sebesseg = 0;
		}
		return sebesseg;
	}
	else return 0;
}

//void debug()
//{
//	Serial.print("K[0]: ");
//	Serial.print(kontrollerSebesseg);
//	Serial.print(" K[1]: ");
//	Serial.print(kontrollerKormany);
//	Serial.print(" K[2]: ");
//	Serial.print(kontrollerGomb);
//	Serial.print(" OTime: ");
//	Serial.print(kontrollerOfflineTimer);
//	Serial.print(" KOnline: ");
//	Serial.print(kontrollerOnline);
//	Serial.print(" ELok: ");
//	Serial.print(elsoLokator);
//	Serial.print(" HLok: ");
//	Serial.println(hatsoLokator);
//}

void setup()
{
	pinMode(MOTOR_ELORE, OUTPUT);
	pinMode(MOTOR_HATRA, OUTPUT);
	pinMode(MOTOR_ENABLE, OUTPUT);
	pinMode(PIEZZO, OUTPUT);
	Serial.begin(9600);
	Wire.begin();
	// Radio bekapcsolasa
	radio.begin();
	// csatorna nyitasa adatok fogadasahoz a taviranyitotol
	radio.openReadingPipe(0, 96);
	// Radio tererejenek minimumra allitasa
	radio.setPALevel(RF24_PA_MIN);
	// Szervo csatlakoztatasa
	kormany.attach(SERVO);
	// kormany kiegyenesitese
	kormany.write(90);
}

void loop()
{
	mozgas();
}
