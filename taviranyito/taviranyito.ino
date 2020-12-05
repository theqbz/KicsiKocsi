/*
 Name:		kicsikocsi.ino
 Created:	12/5/2020 6:14:38 PM
 Author:	qbz
*/

//KICSIKOCSI TÁVIRÁNYÍTÓ


#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <MPU6050.h>


byte RfCS = 7;
byte RfCE = 8;
byte RfMOSI = 11;
byte RfMISO = 12;
byte RfSCK = 13;

byte MINT = 2;
byte MSDA = A4;
byte MSCL = A5;


void setup() {

}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
