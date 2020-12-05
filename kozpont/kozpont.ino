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

boolean masikgomb = 0;					// meg van-e nyomva a t�vir�nyit� gombja
int utasitas = 0;					// a soros portr�l �rkez� utas�t�s
const byte led = 2;

RF24 radio(7, 8);
const byte adas = 2;					// c�m ad�shoz
const byte vetel = 1;					// c�m v�telhez

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	radio.begin();						// ad� ind�t�sa
	radio.openWritingPipe(adas);		// csatorna be�ll�t�sa k�ld�shez
	radio.openReadingPipe(1, vetel);	// csatorna be�ll�t�sa v�telhez
	radio.setPALevel(RF24_PA_MIN);		// ad� t�rereje

}

void loop() {
	radio.startListening();							// vev� m�dba kapcsolja az ad�t
	if (radio.available()) {						// ha j�tt �zenet...
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

	radio.stopListening();							// ad� m�dba kapcsolja az ad�t
	utasitas = Serial.read();						// kiolvassa az utas�t�st soros portr�l
	if (utasitas == 1)
	{
		radio.write(&utasitas, sizeof(utasitas));	// elk�ldi az utas�t�st
	}
	delay(5);
}