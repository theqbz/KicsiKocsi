#pragma once

#define TIMEOUT 100			// riaszt�s, ha ennyi cikluson �t nem �zen a t�vir�ny�t�
#define AKADALY 500			// enn�l k�zelebb (kb. 5-6 cm ne menjen akad�lyokhoz)
#define MIN_SEBESSEG 100	// a limit alatti �rt�kn�l nem indul a motor

class Kocsi {
public:
	Kocsi(byte motorEnablePin, byte motorInput1Pin, byte motorInput2Pin, byte piezzoPin, RF24 radio, Servo servo);
	void mozgas();
	String debug();

private:
	const byte _mEnablePin;
	const byte _mElorePin;
	const byte _mHatraPin;
	const byte _piezzoPin;
	RF24 _radio;
	Servo _kormany;

	byte kontrollerSebesseg;
	byte kontrollerKormany;
	bool kontrollerGomb;
	int kontrollerOfflineTimer;
	bool kontrollerOnline;
	byte elsoLokator;
	byte hatsoLokator;

	void lokatorOlvasasa();
	void kontrollerOlvasasa();
	void radioAdatokFeldolgozasa();
	void nincsKapcsolat();
	void lettKapcsolat();
	int sebessegSzamitasa();

};


Kocsi::Kocsi(byte motorEnablePin, byte motorInput1Pin, byte motorInput2Pin, byte piezzoPin, RF24 radio, Servo servo)
	: _mEnablePin(motorEnablePin), _mElorePin(motorInput1Pin),
	_mHatraPin(motorInput2Pin), _piezzoPin(piezzoPin),
	_radio(radio), _kormany(servo)
{

}
