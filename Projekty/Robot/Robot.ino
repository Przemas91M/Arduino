/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10
#define Swiatlo A0
// Obiekt shielda
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *KoloLewe = AFMS.getMotor(1);
Adafruit_DCMotor *KoloPrawe = AFMS.getMotor(2);
// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[6];  // 3 element array holding Joystick readings'
int VLewy;
int VPrawy;
void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  delay(20);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  delay(20);
  AFMS.begin();
  delay(20);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  pinMode(5, OUTPUT);
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  if ( radio.available() )
  {
    // Read the data payload until we've received everything
    bool done = false;
    while (!done)
    {
      // Fetch the data payload
      done = radio.read( joystick, sizeof(joystick) );
      Serial.print("X = ");
      Serial.print(joystick[0]);
      Serial.print(" Y = ");
      Serial.print(joystick[3]);
      Serial.print(" Z = ");
      Serial.println(joystick[4]);
      if (joystick[4] == 112)
      {
        tone(5, 250, 100);
      }
      if ((joystick[0] <= 500) && (joystick[3] <= 499))
      {
        SkrLewoTyl();
      }
      else if ((joystick[0] <= 499) && (joystick[3] >= 515))
      {
        SkrPrawoTyl();
      }
      else if ((joystick[0] >= 511) && (joystick[3] >= 515))
      {
        SkrPrawoPrzod();
      }
      else if ((joystick[0] >= 511) && (joystick[3] <= 499))
      {
        SkrLewoPrzod();
      }
      else if (joystick[3] <= 499)
      {
        SkrLewo();
      }
      else if (joystick[3] >= 515)
      {
        SkrPrawo();
      }
      else if (joystick[0] >= 511) 
      {
        DoPrzodu();
      }
      else if (joystick[0] <= 500) 
      {
        DoTylu();
      }

      else
      {
        KoloPrawe->run(RELEASE);
        KoloLewe->run(RELEASE);
      }
    }
    }
  
  else
  {
    Serial.println("No radio available");
    KoloLewe->run(RELEASE);
    KoloPrawe->run(RELEASE);
    tone(5,750,100);
    delay(1000);
  }


}
void DoPrzodu()
{
  VPrawy = map(joystick[0], 510, 1023, 0, 233);
  VLewy = map(joystick[0], 510, 1023, 0, 255);
  KoloPrawe->setSpeed(VPrawy);
  KoloLewe->setSpeed(VLewy);
  KoloLewe->run(FORWARD);
  KoloPrawe->run(FORWARD);
}
void DoTylu()
{
  VPrawy = map(joystick[0], 500, 0, 0, 233);
  VLewy = map(joystick[0], 500, 0, 0, 255);
  KoloPrawe->setSpeed(VPrawy);
  KoloLewe->setSpeed(VLewy);
  KoloLewe->run(BACKWARD);
  KoloPrawe->run(BACKWARD);
}
void SkrPrawoPrzod()
{
  VPrawy = map(joystick[3], 515, 1023, 120, 0);
  VLewy = map(joystick[3], 515, 1023, 170, 255);
  KoloPrawe->setSpeed(VPrawy);
  KoloLewe->setSpeed(VLewy);
  KoloLewe->run(FORWARD);
  KoloPrawe->run(FORWARD);
}
void SkrLewoPrzod()
{
  VPrawy = map(joystick[3], 500, 0, 170, 233);
  VLewy = map(joystick[3], 500, 0, 170, 0);
  KoloPrawe->setSpeed(VPrawy);
  KoloLewe->setSpeed(VLewy);
  KoloLewe->run(FORWARD);
  KoloPrawe->run(FORWARD);
}
void SkrLewoTyl()
{
  VPrawy = map(joystick[3], 500, 0, 170, 233);
  VLewy = map(joystick[3], 500, 0, 170, 0);
  KoloPrawe->setSpeed(VPrawy);
  KoloLewe->setSpeed(VLewy);
  KoloLewe->run(BACKWARD);
  KoloPrawe->run(BACKWARD);
}
void SkrPrawoTyl()
{
  VPrawy = map(joystick[3], 515, 1023, 170, 0);
  VLewy = map(joystick[3], 515, 1023, 170, 255);
  KoloPrawe->setSpeed(VPrawy);
  KoloLewe->setSpeed(VLewy);
  KoloLewe->run(BACKWARD);
  KoloPrawe->run(BACKWARD);
}
void SkrLewo()
{
  VPrawy = map(joystick[3], 500, 0, 0, 150);
  VLewy = 0;
  KoloPrawe->setSpeed(VPrawy);
  KoloLewe->setSpeed(VLewy);
  KoloLewe->run(FORWARD);
  KoloPrawe->run(FORWARD);
}
void SkrPrawo()
{
  VPrawy = 0;
  VLewy = map(joystick[3], 515, 1023, 0, 150);
  KoloPrawe->setSpeed(VPrawy);
  KoloLewe->setSpeed(VLewy);
  KoloLewe->run(FORWARD);
  KoloPrawe->run(FORWARD);
}



//*********( THE END )***********





