void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}
bool isAlive = true;
int ind;
uint8_t r,g,b;
String message = "100,2,45";
void loop() {
  // put your main code here, to run repeatedly:
  while (isAlive)
  {
    ind = message.indexOf(',');
    r = (uint8_t)atoi(message.substring(0,ind).c_str());
    message = message.substring(ind + 1, message.length());
    ind = message.indexOf(',');
    g = (uint8_t)atoi(message.substring(0,ind).c_str());
    message = message.substring(ind + 1, message.length());
    b = (uint8_t)atoi(message.c_str());
    Serial.println("");
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
    isAlive = false;
  }
}
