#define swPin 4  //D4
#define dtPin 2  //D5
#define clkPin 3 //D6 

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

long readEncoderValue(void){
    return encoderValue/4;
}

boolean isButtonPushDown(void){
  if(!digitalRead(swPin)){
    delay(5);
    if(!digitalRead(swPin))
      return true;
  }
  return false;
}

void setup() {
  Serial.begin (9600);

  pinMode(dtPin, INPUT);
  pinMode(clkPin, INPUT);
  pinMode(swPin, INPUT_PULLUP);
  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(digitalPinToInterrupt(2), updateEncoder, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(3), updateEncoder, CHANGE);

}

void loop(){
  //Do stuff here

  if(isButtonPushDown()){
    Serial.println("you push button down!!!");
  }
  Serial.println(readEncoderValue());
  delay(50); //just here to slow down the output, and show it will work  even during a delay
}


void updateEncoder(){
  int MSB = digitalRead(clkPin); //MSB = most significant bit
  int LSB = digitalRead(dtPin); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue   ;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
}
