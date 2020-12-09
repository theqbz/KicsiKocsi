/*
 Name:		lokator.ino
 Created:	12/9/2020 5:03:01 PM
 Author:	qbz
*/

#define Trig 2
#define Echo 3

long ido;
int tavolsag;

// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(Trig, OUTPUT);
	pinMode(Echo, INPUT);
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	digitalWrite(Trig, LOW);
	delayMicroseconds(2);
	digitalWrite(Trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(Trig, LOW);
	ido = pulseIn(Echo, HIGH);
	tavolsag = ido * 0.034 / 2;
	Serial.print(tavolsag);
	Serial.println(" cm");
}
