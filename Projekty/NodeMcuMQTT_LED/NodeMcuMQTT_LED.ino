#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 5
#define LED_COUNT 46
#define TRIG_PIN 14
#define ECHO_PIN 12

#define wifi_ssid "Domek_33_3G"
#define wifi_password "281128MO"

#define mqtt_server "192.168.1.3"
#define mqtt_user ""
#define mqtt_password ""

#define sterowanie_topic "Home/Biurko/Rozkazy"
#define stan_topic "Home/Biurko/Info"
bool auto_reczny = true;
bool wykonajProgram = false;
bool led = false;
bool ledState = false;
int programLed = 0;
int dystans = 0;
int pomiar, licznik, licznikRandom;
String stan = "0"; //0 - tęcza, 1 - stały kolor
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;
long firstPixelHue = 0;
long interruptLed = 0;
long interruptRandom = 0;
long interruptDist = 0;
long randomNumber;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
WiFiClient espClient;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient client(mqtt_server, 1883, callback, espClient);

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  //interpretacja informacji wejściowych - trybu auto/manual, wybranego koloru, wybranego programu
  String message;
  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  if (String(topic) == "Home/Biurko/Rozkazy") //otrzymalem rozkaz
  {
    if (message == "1") //tryb auto
    {
      auto_reczny = true;
      Serial.println("Tryb auto");
    }
    else if (message == "0") //tryb reczny
    {
      auto_reczny = false;
      wykonajProgram = true;
      Serial.println("Tryb reczny");
    }
    else if (message == "c") // zmiana programu led
    {
      programLed++; //zmien program
      if (programLed > 4) //jak na razie mam 4 programy
      {
        programLed = 0; //zmień na zero jak będe miec 3
        Serial.println("Tecza");
        client.publish(stan_topic, "0");
      }
      else if (programLed == 1)
      {
        Serial.println("Kolor");
        client.publish(stan_topic, "1");
      }
      else if (programLed == 2)
      {
        Serial.println("Tylko nogi");
        client.publish(stan_topic, "2");
      }
      else if (programLed == 3)
      {
        Serial.println("Tylko góra");
        client.publish(stan_topic, "3");
      }
      else if (programLed == 4)
      {
        Serial.println("Losowy kolor");
        client.publish(stan_topic, "4");
      }
      wykonajProgram = true; // trigger uruchamiajacy program
    }
    else if (payload[0] == 'b') //zmiana jasności
    {
      int bright = message.substring(1, message.length()).toInt();
      Serial.println(bright);
      strip.setBrightness(bright);
      strip.show();
    }
    else //otrzymaliśmy zmianę koloru 254,7,24
    {
      //przygotowuje kolory
      int ind = message.indexOf(',');
      r = (uint8_t)atoi(message.substring(0, ind).c_str());
      message = message.substring(ind + 1, message.length());
      ind = message.indexOf(',');
      g = (uint8_t)atoi(message.substring(0, ind).c_str());
      message = message.substring(ind + 1, message.length());
      b = (uint8_t)atoi(message.c_str());
      wykonajProgram = true; // trigger odpalajacy program (gdy mamy pojedynczy kolor etc)
      //w przypadku tęczy ten rozkaz nie będzie mieć efektu, lecz zostanie zapamiętany - przypisany do globalnych zmiennych
      //jeżeli zmienimy program to te zmienne zostaną pobrane i utworzą kolor

    }
  }
}


//client.publish(state_topic, "ON", 1);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    if (client.connect("ESP8266Client")) {
      //if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(sterowanie_topic);
      Serial.println("Subskrypcja");
      client.publish(stan_topic, "0");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(10);
  long now = millis();
  if (auto_reczny) //tryb automatyczny - uruchamia pasek gdy ktos siedzi przy biurku
  {
    if (now - interruptDist > 1000) //pomiar odległości
    {
      interruptDist = now;
      dystans = zmierzOdleglosc();
      Serial.println(dystans);
      Serial.println(licznik);
      if (dystans < 25 && licznik <= 5) { //ktos sie pojawił
        licznik++;      //czekamy z pomiarem
        led = false;
      }
      else if (dystans < 25 && licznik > 5) { //mamy kogoś w polu widzenia
        led = true; //praca paska led na podstawie wybranego programu
      }
      //else if (dystans > 25 && licznik > 5) - jezeli ktos zniknal z pola widzenia a ledy dzialaja, poczekac 5 sekund, potem wykasowac licznik
      else              //ktos wybył - dopisać czekanie 5 sekund zanim zgasną ledy
      {
        led = false;
        licznik = 0;
        strip.clear();
        strip.show();
      }
    }
    if(ledState != led)
    {
      wykonajProgram = true;
      ledState = led;
    }
  }
  else { //tryb reczny - pasek dziala na podstawie wybranego programu caly czas
    led = true; //ciagla praca paska led na podstawie wybranego programu
  }
  //jezeli bool jest true, pasek jest uruchomiony i wykonuje swoj program ze swoim timerem
  if (led) {
    if (programLed == 0) {
      //tecza
      if (now - interruptLed > 50) {
        interruptLed = now;
        rainbow(1);
      }
    }
    else if (programLed == 1 && wykonajProgram)
    { //jezeli mamy caly pasek
      colorWipe(strip.Color(r, g, b), 100);
      wykonajProgram = false;
    }
    else if (programLed == 2 && wykonajProgram)
    { //jezeli mamy wybrane tylko nogi
      colorWipe(strip.Color(r , g , b), 23, 100);
      wykonajProgram = false;
    }
    else if (programLed == 3 && wykonajProgram)
    { //wybrana tylko góra
      colorWipe(strip.Color(r, g, b), 0, 23, 100);
      wykonajProgram = false;
    }
    else if (programLed == 4) { //losowy kolor co 5 sekund
      if (now - interruptRandom > 5000) {
        interruptRandom = now;
        randomNumber = random(255);
        r = (int)randomNumber;
        randomNumber = random(255);
        g = (int)randomNumber;
        randomNumber = random(255);
        b = (int)randomNumber;
        colorWipe(strip.Color(r, g, b), 100);
        Serial.print(r);
        Serial.print(g);
        Serial.println(b);
      }
    }
  }
}
int zmierzOdleglosc() {
  long czas, dystans;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  czas = pulseIn(ECHO_PIN, HIGH);
  dystans = czas / 58;

  return dystans;
}
//funkcje paska led
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
//uaktywnamy kolor tylko od wybranego piksela (funkcja tylko dla nóg)
void colorWipe(uint32_t color, int pocz, int wait) {
  uint32_t zero = strip.Color(0, 0, 0);
  for (int i = 0; i < pocz; i++) { // For each pixel in strip...
    strip.setPixelColor(i, zero);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
  for (int i = pocz; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
//uaktywnamy kolor od zera do wybranego piksela (funkcja tylko dla góry)
void colorWipe(uint32_t color, int pocz, int koniec, int wait) {
  uint32_t zero = strip.Color(0, 0, 0);
  for (int i = pocz; i < koniec; i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
  for (int i = koniec; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, zero);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void rainbow(int wait) {
  //for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
  if (firstPixelHue < 5 * 65536)
  {
    for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    firstPixelHue += 256;
  }
  else
  {
    firstPixelHue = 0;
  }
  strip.show(); // Update strip with new contents
  delay(wait);  // Pause for a moment
}
