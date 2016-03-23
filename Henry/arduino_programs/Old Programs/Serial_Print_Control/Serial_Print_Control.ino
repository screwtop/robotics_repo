const int Switch1Pin = 2;
const int Switch2Pin = 3;
int Range_Threshold = 50; //cm
const int Ledpin = 19;
const int Bumpleft = 14;
const int Bumpright = 15;

const int MOTOR_L_ENABLE_PIN = 11, //left
MOTOR_L_A_PIN = 6,
MOTOR_L_B_PIN = 7;

const int MOTOR_R_ENABLE_PIN = 5, //right
MOTOR_R_A_PIN = 9,
MOTOR_R_B_PIN = 10;

int URPWM = 17;  //Correct , PWM Output 0－25000US，Every 50US represent 1cm
int URTRIG = 8; //Correct 

const float MIN_SPEED_FACTOR = 1.0;
float RIGHT_MOTOR_SPEED_FACTOR = 1.0;
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

  analogWrite(MOTOR_R_ENABLE_PIN, 0.8 * MAX_PWM_DUTY);
  analogWrite(MOTOR_L_ENABLE_PIN, 0.8 * MAX_PWM_DUTY); 
}
void loop(){
 if (Serial.available()){
      char Commandchar = Serial.read();
      if (Commandchar == 'W')
        forward(MAX_PWM_DUTY);
        Serial.write("I am going forwards!")
      else if (Commandchar == 'Q');
        brake();
        Serial.write("I have stoped");
      else if (Commandchar == 'A')
        spin_left();
        Serial.write("I am spinning left");
      else if (Commandchar == 'D')
        spin_right();
        Serial.write("I am spinning right");
      else if (Commandchar == 'S')
        reverse(MAX_PWM_DUTY);
        Serial.write("I am going backwards!");
      else{
      }
    }
  }


