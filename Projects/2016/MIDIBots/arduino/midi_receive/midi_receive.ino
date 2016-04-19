/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

#define GREEN 8
#define RED   10
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
// const int greenLed = 8,
//          redLed   = 10;

int dataByte[2], statusByte = 0, i = 0;

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(31250);
  // initialize the digital pin as an output.
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);     
  clearData();
}

void clearData(){
  dataByte[0] = 0;
  dataByte[1] = 0;
  i = 0;
}

void flash(int color){
  digitalWrite(color, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10);               // wait for a second
  digitalWrite(color, LOW);    // turn the LED off by making the voltage LOW
  delay(20);               // wait for a second
}

// the loop routine runs over and over again forever:
void loop() {
  if(Serial.available() > 0){
    int data = Serial.read();
    if(data > 127){
      statusByte = data;
      clearData();  
    }else{
      dataByte[i] = data;
      if(statusByte == 0x90 && i == 1){
        if(dataByte[1] == 0){
          flash(RED);
        }else{
          flash(GREEN);
        }
      }
      else if (statusByte == 0x80 && i == 1){
        flash(RED);
      }
      i++; 
    }
  }
}
