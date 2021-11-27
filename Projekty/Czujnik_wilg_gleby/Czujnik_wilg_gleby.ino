#include <DHT.h>
//definiowanie stałych
#define DHTPIN 2
#define DHTTYPE DHT11
//definicja czujnika
DHT dht11(DHTPIN, DHTTYPE);
//zmienne
int Wilgotnosc_gleba;
int Temperatura;
int Wilgotnosc;
char Rozkaz[4];
int Adres[3];
int Adres_sonda[3];
char Komenda;
int i;
int licznik = 0;
void setup() {
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  Serial.begin(9600);
  dht11.begin();
  //sprawdz jaki jest adres sondy na zworce i napisz w serialu
  Sprawdz_adres();
}
void loop() {
  //sprawdzam czy cos moge odczytac
  if (Serial.available() > 0)
  {
    //czytam rozkaz
    Serial.readBytes(Rozkaz, 4);
    //przepisuje adres
    for (i = 0; i < 4; i++)
    {
      Adres[i] = Rozkaz[i] - '0';
    }
    //sprawdzam, czy to rozkaz dla mojego urządzenia
    for (i = 0; i < 4; i++)
    {
      if (Adres[i] == Adres_sonda[i])
      {
        licznik++;
      }
    }
    //jeżeli tak, to czytam rozkaz
    if (licznik == 3)
    {
      Komenda = Rozkaz[3];
      if (Komenda == 'a')
      {
        Odczyt_wszystko();
      }
      else if (Komenda == 't')
      {
        Odczyt_temperatura();
      }
      else if (Komenda == 'h')
      {
        Odczyt_wilgotnosc();
      }
      else if (Komenda == 'g')
      {
        Odczyt_gleba();
      }
      else
      {
        Serial.println("Niepoprawny rozkaz");
      }
    }
    else
    {
      Serial.println("Otrzymano rozkaz nie do tego urzadzenia");
    }
  }
}
//funkcja odpowiedzialna za odczyt wilgoci z czujnika gleby
void Odczyt_gleba(void) {
  Wilgotnosc_gleba = analogRead(A7);
  //Serial.print("Wilgotnosc gleby wynosi");
  int Wilgotnosc_procenty = map(Wilgotnosc_gleba,0,1023,0,100);
  Serial.println(Wilgotnosc_procenty);
  licznik = 0;
}
//funkcja odpowiedzialna za odczyt temperatury z czujnika dht11
void Odczyt_temperatura(void) {
  Temperatura = dht11.readTemperature();
  //Serial.print("Aktualna temperatura wynosi: ");
  Serial.println(Temperatura);
  //Serial.println(" st.C.");
  licznik = 0;
}
//funkcja odpowiedzialna za odczyt wilgoci z czujnika dht11
void Odczyt_wilgotnosc(void) {
  Wilgotnosc = dht11.readHumidity();
  //Serial.print("Wilgotnosc powietrza wynosi: ");
  Serial.println(Wilgotnosc);
  //Serial.println(" %");
  licznik = 0;
}
//odczyt danych ze wszystkiego na raz
void Odczyt_wszystko(void) {
  Odczyt_gleba();
  delay(50);
  Odczyt_temperatura();
  delay(50);
  Odczyt_wilgotnosc();
  delay(50);
}
void Sprawdz_adres(void) {
  int pin1 = digitalRead(3);
  int pin2 = digitalRead(4);
  int pin3 = digitalRead(5);
  Adres_sonda[0] = pin1;
  Adres_sonda[1] = pin2;
  Adres_sonda[2] = pin3;
  Serial.print("Adres sondy to: ");
  Serial.print(Adres_sonda[0]);
  Serial.print(Adres_sonda[1]);
  Serial.println(Adres_sonda[2]);
}

