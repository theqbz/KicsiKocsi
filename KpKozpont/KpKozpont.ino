/*
 Name:		KpKozpont.ino
 Created:	1/9/2021 11:06:05 PM
 Author:	qbz
*/
#include <Arduino.h>
#include <Wire.h>

int kuldottAdatok = 0;
byte fogadottAdatok[7];

void setup() {
	Wire.begin();
	Serial.begin(9600);
}

void loop() {
	Wire.beginTransmission(0x09);
	Wire.write(kuldottAdatok);
	Wire.endTransmission();
	Wire.requestFrom(0x09, 7);
	if (Wire.available()) {
		fogadottAdatok[0] = Wire.read();
		fogadottAdatok[1] = Wire.read();
		fogadottAdatok[2] = Wire.read();
		fogadottAdatok[3] = Wire.read();
		fogadottAdatok[4] = Wire.read();
		fogadottAdatok[5] = Wire.read();
		fogadottAdatok[6] = Wire.read();
	}
	for (int i = 0; i < 7; i++) {
		Serial.print(fogadottAdatok[i]);
		Serial.print(" | ");
	}
	Serial.println(" |");
}
