#include <LiquidCrystal.h>
//LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
LiquidCrystal lcd(13, 12, 5, 4, 3, 2);
#define pinGora 11
#define pinEnter 10
#define pinDol 9
/******zmienne*******/
//poziom menu
int poziom;
//zadana temperatura
int temp;
//temperatura odczytana z pinu analogowego 
int celsjusze;
//obsługa klawiszy
int Up;
int Down;
int Enter;
//parametry PID
int P;
int I;
int D;
/*poziomy menu:
0-podglad
1-menu glowne
2-zmiana temperatury
3-wybor czlonu pid
4-czlon P
5-czlon I
6-czlon D
*/
//funkcja wyswietlajaca temperature z pinu analogowego(homescreen)
//enterem wchodzisz w menu
void podglad(){
  while(poziom == 0){
    klawisze();
    //przejdz do menu glownego
    if(Enter == HIGH){
      poziom = 1;}
    celsjusze = analogRead(A0);
    lcd.clear();
    lcd.print("Temperatura:");
    lcd.setCursor(0,1);
    lcd.print(celsjusze);
    delay(150);
  }
}
//menu glowne - wybor ustawienia temperatury badz parametrow PID
//enterem wracasz do podgladu
//gora to wybor temperatury
//dol to wybor czlonu PID
void menu(){
  while(poziom == 1){
    klawisze();
    //przejdz do podgladu
    if(Enter == HIGH){
      poziom = 0;}
    //przejdz do ustawiania temperatury docelowej
    else if(Up == HIGH){
      poziom = 2;}
    //przejdz do ustawiania parametrów członów PID
    else if(Down == HIGH){
      poziom = 3;}
      lcd.clear();
      lcd.print("Ustaw temp.");
      lcd.setCursor(0,1);
      lcd.print("Ustaw PID");
      delay(150);
  }
}
//menu ustawiania temperatury docelowej
//gora/dol ustawiasz
//enterem wychodzisz
void temperatura(){
  while(poziom == 2){
    klawisze();
    //przejdz do menu
    if(Enter == HIGH){
      poziom = 1;}
    //zwieksz temperature
    else if(Up == HIGH){
      temp++;}
    //zmniejsz temperature
    else if(Down == HIGH){
      temp--;}
    lcd.clear();
    lcd.print("Ustaw temp:");
    lcd.setCursor(0,1);
    lcd.print(temp,DEC);
    delay(150);
  }
}
//menu wyboru członu
//1 to dol, 2 to enter, 3 to gora
void PID(){
  lcd.clear();
  lcd.print("Wybierz czlon");
  lcd.setCursor(0,1);
  //1 to dol, 2 to enter, 3 to gora
  lcd.print("1.P  2.I  3.D");
  while(poziom == 3){
    klawisze();
    //przejdz do czlonu P
    if(Up == HIGH){
      poziom = 4;}
    //przejdz do czlonu I
    else if(Enter == HIGH){
     poziom = 5;}
    //przejdz do czlonu D
    else if(Down == HIGH){
     poziom = 6;} 
     delay(150);
  }
}
//ustawianie wartosci czlonu P
//klawiszem enter wychodzisz
//gora/dol regulujesz
void czlonP(){
  while(poziom == 4){
  klawisze();
    //przejdz do menu
    if(Enter == HIGH){
      poziom = 1;}
    //zwieksz temperature
    else if(Up == HIGH){
      P++;}
    //zmniejsz temperature
    else if(Down == HIGH){
      P--;}  
  lcd.clear();
  lcd.print("Ustaw wartosc:");
  lcd.setCursor(0,1); 
  lcd.print(P,DEC);
  delay(150);
  }
}
//czlon I
//to samo co w czlonie P
void czlonI(){
  while(poziom == 5){
    klawisze();
    if(Enter == HIGH){
      poziom = 1;}
    else if(Up == HIGH){
      I++;}
    else if(Down == HIGH){
      I--;}
  lcd.clear();
  lcd.print("Ustaw parametr:");
  lcd.setCursor(0,1);
  lcd.print(I,DEC);
  delay(150);
  }
}
//czlon D
//to samo co wyzej
void czlonD(){
  while(poziom == 6){
    klawisze();
    if(Enter == HIGH){
      poziom = 1;}
    else if(Up == HIGH){
      D++;}
    else if(Down == HIGH){
      D--;}
  lcd.clear();
  lcd.print("Ustaw parametr:");
  lcd.setCursor(0,1);
  lcd.print(D,DEC);
  delay(150);  
  }
}  
//funkcja czytajaca klawisze
void klawisze(){
 Up = digitalRead(pinGora);
 Down = digitalRead(pinDol);
 Enter = digitalRead(pinEnter);
}
void setup() {
  lcd.begin(16,2);//okreslanie ile kolumn,wierszy ma wyswietlacz
  lcd.print("Ogniwo");
  lcd.setCursor(0,1);//ustawia kursor w kolumnie.linijce
  lcd.print("peltiera");
  delay(1000);
}

void loop() {
//tutaj switch na podstawie ustawienia zmiennej poziom
//przelacza sie do konkretnych funkcji
//kazda funkcja dziala w while 
//wiec nie zasmieca loopa
switch(poziom){
  case 0:
  podglad();
  break;
  case 1:
  menu();
  break;
  case 2:
  temperatura();
  break;
  case 3:
  PID();
  break;
  case 4:
  czlonP();
  break;
  case 5:
  czlonI();
  break;
  case 6:
  czlonD();
  break;
}
}


