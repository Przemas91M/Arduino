/*-----( Import needed libraries )-----*/
#include<Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9
#define CSN_PIN 10
#define JOYSTICK1_X A0
#define JOYSTICK1_Y A1
#define JOYSTICK2_Y A2
#define JOYSTICK2_X A3
// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[6];  // 6 element array holding Joystick readings
int przycisk1;
int przycisk2;
int rozmiar;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  radio.begin();
  radio.openWritingPipe(pipe);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  joystick[0] = analogRead(JOYSTICK1_X);
  joystick[1] = analogRead(JOYSTICK1_Y);
  joystick[2] = analogRead(JOYSTICK2_X);
  joystick[3] = analogRead(JOYSTICK2_Y);
  przycisk1 = digitalRead(2);
  przycisk2 = digitalRead(3);
  if (przycisk1 == HIGH)
  {
    joystick[4] = 111;
  }
  else
  {
    joystick[4] = 112;
  }
  if (przycisk2 == HIGH)
  {
    joystick[5] = 111;
  }
  else
  {
    joystick[5] = 112;
  }
  radio.write( joystick, sizeof(joystick) );
}

