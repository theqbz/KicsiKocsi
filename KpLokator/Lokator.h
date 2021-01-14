#pragma once
#define OSSZES_MINTAVETEL 10		// Mintavételek száma a mozgóátlaghoz

class Lokator {
public:
	Lokator(byte triggerPin, byte echoPin);
	int getTavolsag();
	void sDebug();

private:
	const byte _triggerPin;
	const byte _echoPin;
	long meresekEredmenye[OSSZES_MINTAVETEL];
	byte meresID;
	byte meresekSzama;

	long atlagSzamitasa();
	long meres();
};


Lokator::Lokator(byte triggerPin, byte echoPin) :_triggerPin(triggerPin), _echoPin(echoPin), meresekSzama(0), meresID(0)
{
	for (int i = 0; i < OSSZES_MINTAVETEL; i++) meresekEredmenye[i] = 0;
}

int Lokator::getTavolsag()
{
	return int(atlagSzamitasa() * 0.034 / 2);
}

void Lokator::sDebug()
{
	Serial.print("T");
	Serial.print(_triggerPin);
	Serial.print(" E");
	Serial.print(_echoPin);
	Serial.print(" | getTav:");
	Serial.print(getTavolsag());
	Serial.print(" | MSz= ");
	Serial.print(meresekSzama);
	Serial.print(" MID: ");
	Serial.print(meresID);
	Serial.print(" | Me[i] ");
	for (int i = 0; i < OSSZES_MINTAVETEL; i++) {
		Serial.print(i);
		Serial.print(".: ");
		Serial.print(meresekEredmenye[i]);
		Serial.print(" ");
	}
	Serial.println("");
}

long Lokator::atlagSzamitasa()
{
	meresekEredmenye[meresID] = meres();
	if (meresID + 1 < OSSZES_MINTAVETEL) meresID++;
	else meresID = 0;
	long meresekAtlaga = 0.0;
	for (int i = 0; i < meresekSzama; i++) meresekAtlaga += meresekEredmenye[i];
	meresekAtlaga /= meresekSzama;
	return meresekAtlaga;
}

long Lokator::meres()
{
	if (meresekSzama < OSSZES_MINTAVETEL) meresekSzama++;
	digitalWrite(_triggerPin, LOW);
	delayMicroseconds(2);
	digitalWrite(_triggerPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(_triggerPin, LOW);
	return pulseIn(_echoPin, HIGH);
}