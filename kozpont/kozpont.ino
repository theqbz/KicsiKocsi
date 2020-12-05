/*
 Name:		kozpont.ino
 Created:	12/5/2020 8:17:27 PM
 Author:	qbz
*/

#include <SPI.h>
#include <RF24_config.h>
#include <printf.h>
#include <RF24.h>
#include <nRF24L01.h>

boolean masikgomb = 0;					// meg van-e nyomva a távirányitó gombja
int utasitas = 0;					// a soros portról érkezõ utasítás
const byte led = 2;

RF24 radio(7, 8);
const byte adas = 2;					// cím adáshoz
const byte vetel = 1;					// cím vételhez

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	radio.begin();						// adó indítása
	radio.openWritingPipe(adas);		// csatorna beállítása küldéshez
	radio.openReadingPipe(1, vetel);	// csatorna beállítása vételhez
	radio.setPALevel(RF24_PA_MIN);		// adó térereje

}

void loop() {
	radio.startListening();							// vevõ módba kapcsolja az adót
	if (radio.available()) {						// ha jött üzenet...
		radio.read(&masikgomb, sizeof(masikgomb));	// ..olvasd ki
		if (masikgomb)
		{
			digitalWrite(led, HIGH);
		}
		else
		{
			digitalWrite(led, LOW);
		}
	}
	delay(5);

	radio.stopListening();							// adó módba kapcsolja az adót
	utasitas = Serial.read();						// kiolvassa az utasítást soros portról
	if (utasitas == 1)
	{
		radio.write(&utasitas, sizeof(utasitas));	// elküldi az utasítást
	}
	delay(5);
}