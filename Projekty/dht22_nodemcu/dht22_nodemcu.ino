#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// DHT Sensor
uint8_t DHTPin = 13; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;
 
void setup() {
  Serial.begin(115200);
  delay(100);
  //pinMode(13, OUTPUT);
  pinMode(DHTPin, INPUT);

  dht.begin();              

 
}
void loop() {
  delay(2000);
 Temperature = dht.readTemperature(); // Gets the values of the temperature
 Humidity = dht.readHumidity(); // Gets the values of the humidity 
Serial.println(Temperature);
Serial.println(Humidity);
delay(1000); 
  
}
