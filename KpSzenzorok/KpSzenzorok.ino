/*
 Name:		KpSzenzorok.ino
 Created:	1/9/2021 7:59:35 PM
 Author:	qbz
*/

#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <Wire.h>

#define HC1Echo 2				// Els� HC-SR04-4P Echo
#define HC1Trig 3				// Els� HC-SR04-4P Triger
#define HC2Echo 4				// H�ts� HC-SR04 Echo
#define HC2Trig 5				// H�ts� HC-SR04 Triger
#define RfCE 7					// nRF24L01 "Chip Enable"
#define RfCS 8					// nRF24L01 "Chip Set"
#define MOSI 11					// nRF24L01 MOSI (Az ICSP kommunik�ci�hoz sz�ks�ges)
#define MISO 12					// nRF24L01 MISO (Az ICSP kommunik�ci�hoz sz�ks�ges)
#define SCK 13					// nRF24L01 SCK (Az ICSP kommunik�ci�hoz sz�ks�ges)

#define LOKATOROK_SZAMA 4
#define LOKATOR_ATLAG 10		// ennyi m�r�s mozg��tlag�t sz�molja
#define TIMEOUT 100				// riaszt�s, ha ennyi cikluson �t nem �zen a t�vir�ny�t�

RF24 radio(RfCE, RfCS);			// R�di� l�trehoz�sa
const byte cim = 96;			// a R�di� csatorn�j�nak c�me
byte adatokAKozpontbol;			// nem haszn�lt v�ltoz� a vissza ir�ny� kommunikci�ra
byte kuldendo[7];				// adatok a k�zpontba: ok, el�re, oldalra, lok�tor 1-4



class KpSzenzor {
public:
	KpSzenzor(int lokatorokszama)
		:lokatorokSzama(lokatorokszama), kimarad(0), taviranyitoOnline(true) {
		for (int i = 0; i < 3; i++) { csomagTaviranyitotol[i] = 0; }
		for (int i = 0; i < 4; i++) { lokatorAdatok[i] = 0; }
	}

	void setLokatorData(int index, int tavolsag) { lokatorAdatok[index] = tavolsag; }

	void csomagOsszeallitasa(RF24& radio) {
		taviranyitoOlvasasa(radio);
		if (taviranyitoOnline) { kuldendo[0] = getTaviranyitoData(0); }
		else { kuldendo[0] = 0; }
		kuldendo[1] = getTaviranyitoData(1);
		kuldendo[2] = getTaviranyitoData(2);
		kuldendo[3] = getLokatorData(0);
		kuldendo[4] = getLokatorData(1);
		kuldendo[5] = getLokatorData(2);
		kuldendo[6] = getLokatorData(3);
	}

	void debug() {
		String ki = "kimarad: ";
		ki += kimarad;
		if (taviranyitoOnline) { ki += " Online "; }
		else { ki += " offline "; }
		Serial.println(ki);
	}

private:
	const byte lokatorokSzama;
	byte csomagTaviranyitotol[3];	// a t�vir�ny�t�b�l �rkez� adatok (el�re, oldalra, ok)
	byte lokatorAdatok[4];
	byte kimarad;
	bool taviranyitoOnline;

	void taviranyitoOlvasasa(RF24& r) {
		r.startListening();
		if (r.available()) {
			if (!taviranyitoOnline) { lettKapcsolat(); }					// jelz�s, ha most lett �jra online a t�vir�ny�t�
			r.read(&csomagTaviranyitotol, sizeof(csomagTaviranyitotol));	// adatok beolvas�sa
			kimarad = 0;
		}
		else { kimarad++; }
		if (kimarad > TIMEOUT) { nincsKapcsolat(); }
	}

	byte getTaviranyitoData(int index) { return byte(csomagTaviranyitotol[index]); }

	byte getLokatorData(int index) { return byte(lokatorAdatok[index]); }

	void lettKapcsolat() {
		//tone(Csipogo, 880, 40);
		//delay(250);
		//tone(Csipogo, 1760, 40);
		taviranyitoOnline = true;
	}

	void nincsKapcsolat() {
		//tone(Csipogo, 1000, 40);
		//delay(100);
		//tone(Csipogo, 880, 20);
		//delay(100);
		//tone(Csipogo, 880, 20);
		//sebesseg = 0;
		taviranyitoOnline = false;
		kimarad = 0;
	}

};


KpSzenzor ArNano(2);
Lokator elsoLokator(HC1Trig, HC1Echo);
Lokator hatsoLokator(HC2Trig, HC2Echo);


void adatokFogadasa(int n) {
	while (Wire.available()) {
		adatokAKozpontbol = Wire.read();
	}
}

void adatokKuldese() {
	ArNano.setLokatorData(1, elsoLokator.tavolsag());
	ArNano.setLokatorData(2, hatsoLokator.tavolsag());
	ArNano.csomagOsszeallitasa(radio);
	Wire.write(kuldendo, sizeof(kuldendo));
}


void setup() {
	Serial.begin(9600);
	Wire.begin(0x09);
	Wire.onReceive(adatokFogadasa);
	Wire.onRequest(adatokKuldese);
	radio.begin();						// R�di� bekapcsol�sa
	radio.openReadingPipe(0, cim);		// csatorna nyit�sa adatok fogad�s�hoz a t�vir�ny�t�t�l
	radio.setPALevel(RF24_PA_MIN);		// R�di� t�rerej�nek minimumra �ll�t�sa
}

void loop() {
	//ArNano.setLokatorData(1, elsoLokator.tavolsag());
	//ArNano.setLokatorData(2, hatsoLokator.tavolsag());
	ArNano.csomagOsszeallitasa(radio);
	toDebug();

}

void toDebug() {
	for (int i = 0; i < 7; i++)
	{
		Serial.print(kuldendo[i]);
		Serial.print(" | ");
	}
	Serial.print(sizeof(kuldendo));
	Serial.print(" | ");
	ArNano.debug();
}
