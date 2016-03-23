int ledPins[] = { 
  6,7,8,9,10} 
;              
int timer = 35 ;
int pinCount = 5;

void setup()                     
{
for (int thisPin = pinCount - 1; thisPin >= 0; thisPin--) {
   pinMode(ledPins[thisPin], OUTPUT);
   digitalWrite(ledPins[thisPin],HIGH); 
  }  
  delay (1000);
  for (int thisPin = pinCount - 1; thisPin >= 0; thisPin--) {
      digitalWrite(ledPins[thisPin],LOW); 
      delay (250);
  } 
  delay (100);
}
void loop()                      
{
  for (int thisPin = pinCount - 1; thisPin >= 0; thisPin--) {
    digitalWrite(ledPins[thisPin], random (0,2)); 
  }
   delay (timer);
 }


