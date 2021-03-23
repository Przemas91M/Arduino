int odczyt;
int srednia;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(A1, INPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
//  for(int i = 0; i < 10;i++)
//  {
//    odczyt +=analogRead(A1);
//    delay(40);
//  }
//  srednia = odczyt / 10;
Serial.println(analogRead(A1));
//Serial.println(srednia);
//odczyt = 0;
//srednia = 0;
delay(100);
}
