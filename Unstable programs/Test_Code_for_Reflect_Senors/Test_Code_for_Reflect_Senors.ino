int Left_closed = Left_closed;
int Left_open = Left_open;
void setup() {

  Serial.begin(9600);
}


void loop() {

  int Reflect_right = analogRead(A2);
  int Reflect_left = analogRead(A5);
  Serial.println(Reflect_left);
  delay(500);
 // Serial.println(Reflect_right);
 // delay(500);
  
  if (Reflect_left > 40)
{
 Left_open = True
Serial.println(Left_open); 
}
}
