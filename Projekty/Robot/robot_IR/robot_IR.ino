#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#define irPin 2
IRrecv irrecv(irPin);
decode_results results;

// Obiekt shielda
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Definicja portow na ktorych sa silniki
Adafruit_DCMotor *KoloLewe = AFMS.getMotor(1);
Adafruit_DCMotor *KoloPrawe = AFMS.getMotor(2);
String odczyt;
int swiatlo;
boolean irrec;

void setup() {       
  Serial.begin(9600);
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz  
  // Predkosc silnikow 0 (off) to 255 (max speed)
  irrecv.enableIRIn();
  KoloLewe->setSpeed(170);
  KoloPrawe->setSpeed(140);
}
void loop()
{
  swiatlo = analogRead(A0);
  if (swiatlo >=50)
  {
    digitalWrite(3,LOW);
  }
  else
  {
    digitalWrite(3,HIGH);
  }
  if (irrecv.decode(&results))
  {
     odczyt = String(int(results.value),HEX);
     Serial.println(results.value, HEX);
     switch(results.value)
     {
       default:
       KoloLewe->run(RELEASE);
       KoloPrawe->run(RELEASE);
       delay(100);
       break;
       case 0x481:
       KoloLewe->run(FORWARD);
       KoloPrawe->run(FORWARD);
       delay(100);
       break;
       case 0xC81:
       KoloLewe->run(BACKWARD);
       KoloPrawe->run(BACKWARD);
       delay(500);
       KoloLewe->run(RELEASE);
       KoloPrawe->run(RELEASE);
       break;
       case 0x4CB9C:
       KoloLewe->run(FORWARD);
       KoloPrawe->run(RELEASE);
       delay(500);
       KoloLewe->run(RELEASE);
       KoloPrawe->run(RELEASE);
       break;
       case 0x1CB9C:
       KoloLewe->run(RELEASE);
       KoloPrawe->run(FORWARD);
       delay(500);
       KoloLewe->run(RELEASE);
       KoloPrawe->run(RELEASE);
      }
      irrecv.resume();
  }
}
