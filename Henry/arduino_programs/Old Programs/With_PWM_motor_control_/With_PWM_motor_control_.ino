void setup() {                
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);
      
}
//for motor  5 becomes 4, 9 becomes 6 and 10 becomes 7.
void loop() {
    digitalWrite(9,LOW); 
    digitalWrite(10,HIGH); 
    analogWrite(5, 128); 
  delay(1000); 
 analogWrite(5, 0); 
  delay(1000);  
   
}
