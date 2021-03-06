#define outputA 3
#define outputB 2
 volatile int counter = 0; 
 int aState;
 int aLastState; 
  
 void setup() { 
   pinMode (outputA,INPUT);
   pinMode (outputB,INPUT);
   Serial.begin (9600);
   // Reads the initial state of the outputA
   aLastState = digitalRead(outputA);   
   attachInterrupt(0, readEncoder, CHANGE);
   attachInterrupt(1, readEncoder, CHANGE);
 } 
 void loop() { 
  Serial.println(counter);
  delay(50);
  
 }

 void readEncoder() {
   aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter ++;
     } else {
       counter --;
     }
     //Serial.print("Position: ");
     //Serial.println(counter);
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state
 }
