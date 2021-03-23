#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#define relayPin 7 //D7
#define sensorPin 8 //D8
#define swPin 4  //D4
#define dtPin 2  //D2
#define clkPin 3 //D3 
LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire ds(sensorPin);

byte data[12];
byte addr[8];

byte termo[] =
{
  B00100,
  B01010,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110
};

byte grade[] =
{
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void findSensor() {
  ds.reset_search();
  while (!ds.search(addr)) {
    //Serial.println("No more addresses.");
    //Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Blad");
    lcd.setCursor(0, 1);
    lcd.print("czujnika");
    ds.reset_search();
    delay(250);
  }
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Czujnik");
  lcd.setCursor(7, 1);
  lcd.print("OK");
  delay(1000);
}

void clearLine(int line) {
  lcd.setCursor(0, line);
  for (int i = 0; i < 16; i++) {
    lcd.print(" ");
  }
}

void readEncoder(){
  
}


void setup() {
  // put your setup code here, to run once:
  pinMode(dtPin, INPUT);
  pinMode(clkPin, INPUT);
  pinMode(swPin, INPUT);
  attachInterrupt(dtPin, readEncoder, CHANGE);
  attachInterrupt(clkPin, readEncoder, CHANGE); 
  lcd.init();
  lcd.begin(16, 2);
  lcd.createChar(0, termo);
  lcd.createChar(1, grade);
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("Hello");
  delay(2000);
  findSensor();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");
  Serial.begin(9600);

}

void loop(void) {
  byte i;
  byte present = 0;
  float celsius;
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  celsius = (float)raw / 16.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  //clearLine(1);
  lcd.setCursor(0, 1);
  lcd.write(0);
  lcd.setCursor(3, 1);
  lcd.print(celsius);
  lcd.write(1);
  lcd.print("C");
}
