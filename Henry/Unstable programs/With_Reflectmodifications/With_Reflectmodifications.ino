const int Switch1Pin = 2;
const int Switch2Pin = 3;
int Range_Threshold = 50; //cm
const int Ledpin = 19;
const int Bumpleft = 14;
const int Bumpright = 15;
int ReflectTrue = 1;
int DetectedCount = 0;
int WheelRotation = 0;
const int MOTOR_L_ENABLE_PIN = 11, //left
MOTOR_L_A_PIN = 6,
MOTOR_L_B_PIN = 7;

const int MOTOR_R_ENABLE_PIN = 5, //right
MOTOR_R_A_PIN = 9,
MOTOR_R_B_PIN = 10;

int URPWM = 17;  //Correct , PWM Output 0－25000US，Every 50US represent 1cm
int URTRIG = 8; //Correct

const float MIN_SPEED_FACTOR = 1.0;
float RIGHT_MOTOR_SPEED_FACTOR = 1.9;
const int MAX_PWM_DUTY_CONST = 6 / 7.2 * 255; //240 for Carpet,220 for wood
const int SLOW_PWM_DUTY_CONST = 5 / 7.2 * 255 ; //180 for Carpet, 150 for wood
int MAX_PWM_DUTY = MAX_PWM_DUTY_CONST;
int SLOW_PWM_DUTY = SLOW_PWM_DUTY_CONST;

float speed_from_range(unsigned int range){
  return clip( 1.0 / 43 * range, MIN_SPEED_FACTOR, 1 );

}

float clip (float value, float min, float max){
  if (value<min)return min;
  else if (value>max) return max;
  else return value;
}

void setup()
{
  pinMode (Switch2Pin, INPUT);
  pinMode (Switch1Pin, INPUT);
  digitalWrite (Switch1Pin, HIGH); //Enables pullup resistor for the pin
  digitalWrite (Switch2Pin, HIGH);
  delay(4000);
  Serial.begin(9600);
  PWM_Mode_Setup();
  pinMode (Ledpin, OUTPUT);
  pinMode(Bumpleft, INPUT);
  digitalWrite (Bumpleft, HIGH);
  pinMode(Bumpright, INPUT);
  digitalWrite (Bumpright, HIGH);
  pinMode(MOTOR_L_A_PIN, OUTPUT);
  pinMode(MOTOR_L_B_PIN, OUTPUT);

  pinMode(MOTOR_R_A_PIN, OUTPUT);
  pinMode(MOTOR_R_B_PIN, OUTPUT);
}

void forward(int PWM)  {
  digitalWrite(MOTOR_L_A_PIN, LOW);
  digitalWrite(MOTOR_L_B_PIN, HIGH);
  digitalWrite(MOTOR_R_A_PIN, LOW);
  digitalWrite(MOTOR_R_B_PIN, HIGH);
  analogWrite(MOTOR_L_ENABLE_PIN,PWM);
  analogWrite(MOTOR_R_ENABLE_PIN,clip (PWM * RIGHT_MOTOR_SPEED_FACTOR, 0 , 255)); // PWM 220/255 gives about 4.5 V at the motor

}

void reverse(int PWM) {
  digitalWrite(MOTOR_L_A_PIN, HIGH);
  digitalWrite(MOTOR_L_B_PIN, LOW);
  digitalWrite(MOTOR_R_A_PIN, HIGH);
  digitalWrite(MOTOR_R_B_PIN, LOW);
  analogWrite(MOTOR_L_ENABLE_PIN, PWM);
  analogWrite(MOTOR_R_ENABLE_PIN, clip (PWM * RIGHT_MOTOR_SPEED_FACTOR, 0 , 255));
}

void stop() {
  digitalWrite(MOTOR_L_ENABLE_PIN, 0);
  digitalWrite(MOTOR_R_ENABLE_PIN, 0);
}

void brake() {
  digitalWrite(MOTOR_L_ENABLE_PIN, HIGH);
  digitalWrite(MOTOR_R_ENABLE_PIN, HIGH);
  digitalWrite(MOTOR_L_A_PIN, 1);
  digitalWrite(MOTOR_L_B_PIN,1);
  digitalWrite(MOTOR_R_A_PIN, 1);
  digitalWrite(MOTOR_R_B_PIN,1);
}

void right() {
  digitalWrite(MOTOR_L_A_PIN, LOW);
  digitalWrite(MOTOR_L_B_PIN, HIGH);
  digitalWrite(MOTOR_R_A_PIN, LOW);
  digitalWrite(MOTOR_R_B_PIN, HIGH);

  analogWrite(MOTOR_L_ENABLE_PIN, 0.8 * MAX_PWM_DUTY);
  analogWrite(MOTOR_R_ENABLE_PIN, 1.0 * MAX_PWM_DUTY);
}

void spin_right() {
  digitalWrite(MOTOR_L_A_PIN, LOW);
  digitalWrite(MOTOR_L_B_PIN, HIGH); // forward
  digitalWrite(MOTOR_R_A_PIN, HIGH);
  digitalWrite(MOTOR_R_B_PIN, LOW); // reverse

  analogWrite(MOTOR_L_ENABLE_PIN, 0.8 * MAX_PWM_DUTY);
  analogWrite(MOTOR_R_ENABLE_PIN, 0.8 * MAX_PWM_DUTY);
}

void spin_left() {
  digitalWrite(MOTOR_R_A_PIN, LOW);
  digitalWrite(MOTOR_R_B_PIN, HIGH); // forward
  digitalWrite(MOTOR_L_A_PIN, HIGH);
  digitalWrite(MOTOR_L_B_PIN, LOW); // reverse

  analogWrite(MOTOR_R_ENABLE_PIN, 0.8 * SLOW_PWM_DUTY);
  analogWrite(MOTOR_L_ENABLE_PIN, 0.8 * SLOW_PWM_DUTY);
}

void testloop()
{


}

void loop()
{ 

  forward(speed_from_range(Get_Range()) * MAX_PWM_DUTY);

  Serialread();
      Objectdodge();
        if (WallHit() == 1){
          reverse(MAX_PWM_DUTY);
          delay(1000);
        }
          Swithmotortoggle();
            Bumpsensors();

}

void Serialread()
{ 
  {
    if (Serial.available()){
      char Commandchar = Serial.read();
      if (Commandchar == '1')
        RIGHT_MOTOR_SPEED_FACTOR = RIGHT_MOTOR_SPEED_FACTOR + 0.01;
      else if (Commandchar == '2')
        RIGHT_MOTOR_SPEED_FACTOR = RIGHT_MOTOR_SPEED_FACTOR - 0.01;
      if (Commandchar == '+')
        RIGHT_MOTOR_SPEED_FACTOR = RIGHT_MOTOR_SPEED_FACTOR + 0.1;
      else if (Commandchar == '-')
        RIGHT_MOTOR_SPEED_FACTOR = RIGHT_MOTOR_SPEED_FACTOR - 0.1;
      else if (Commandchar == 'W')
        forward(MAX_PWM_DUTY);
      else if (Commandchar == 'Q')
        brake();
      else if (Commandchar == 'A')
        spin_left();
      else if (Commandchar == 'D')
        spin_right();
      else if (Commandchar == 'S')
        reverse(MAX_PWM_DUTY);
      else{
      }
    }
  }
}
void Bumpsensors()
{
  if (!digitalRead(Bumpleft)) {
    reverse(SLOW_PWM_DUTY);
    delay(500);
    spin_right();
    delay(300);
  }
  if (!digitalRead(Bumpright)) {
    reverse(SLOW_PWM_DUTY);
    delay(500);
    spin_left();
    delay(300);	 
  }
}
void Swithmotortoggle()
{
  if (!digitalRead(Switch1Pin)){
    digitalWrite(Ledpin,HIGH);
    SLOW_PWM_DUTY = SLOW_PWM_DUTY_CONST;
    MAX_PWM_DUTY = MAX_PWM_DUTY_CONST;
  }
  else {
    digitalWrite(Ledpin,LOW);
    SLOW_PWM_DUTY = 0;
    MAX_PWM_DUTY = 0;  	 
  }
}

int WallHit(){
  //values is an array with 10 slots (starting at slot 0)
  int values [10];
  int i = 0;
  //while i is less than 10, go through filling array with
  //values from the sensor
  while (i < 10) {
    values[i] = analogRead(A2);
    Serial.println(values[i]);
    i = i + 1;
  }
  i = 0;
  while (i < 10) {
    if (i > 0){
      if (values[i] > values[i - 1] + 50 || values[i] < values[i - 1] - 50){
        return 1;
      }
     }
      i = i + 1;
  }
  return 0;
}

void Objectdodge()
{	
  unsigned int Range = Get_Range();
  //Serial.println (Range);
    // Serial.println ("We're in object dodge mode");
  digitalWrite(Ledpin, HIGH);
  if ((Range < 40) && (Range != 0)){
    //   Serial.println ("object detected within 40 cm");
    spin_left();
    delay (500);
    Range = Get_Range();
    if ((Range < 39) && (Range != 0)){
      Serial.println ("turning right to dodge");
      spin_right();
      delay(500);
    }
  }
}
void PWM_Mode_Setup()
{
  pinMode(URTRIG,OUTPUT);                 	// A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);              	// Set to HIGH

    pinMode(URPWM, INPUT);                  	// Sending Enable PWM mode command

  for(int i=0;i<4;i++)
  {
    //Serial.write(EnPwmCmd[i]);
  }
}

unsigned int Get_Range()
{	 
  unsigned int Distance;  // a low pull on pin COMP/TRIG  triggering a sensor reading
  digitalWrite(URTRIG, LOW);
  digitalWrite(URTRIG, HIGH);	// reading Pin PWM will output pulses

  unsigned long DistanceMeasured=pulseIn(URPWM,LOW);
  //Serial.println("dist:");  
  //  Serial.println (DistanceMeasured);
  if(DistanceMeasured>=10200)
  {          	// the reading is invalid.
    return 0;    
  }
  else {
    Distance=DistanceMeasured/50;   // every 50us low level stands for 1cm
    return Distance;
    Serial.println (Distance);
  }
}







