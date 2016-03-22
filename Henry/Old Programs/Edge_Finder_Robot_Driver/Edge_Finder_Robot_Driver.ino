int Ledpin = 8;
int Bumpleft = 14;
int Bumpright = 15;
const int MOTOR1_ENABLE_PIN = 11,
MOTOR1_A_PIN = 6,
MOTOR1_B_PIN = 7;

const int MOTOR2_ENABLE_PIN = 5,
MOTOR2_A_PIN = 9,
MOTOR2_B_PIN = 10;

const int MAX_PWM_DUTY = 240; // 220 is about right, depending on battery charge level.

void setup()
{
  delay(10000);
  pinMode (Ledpin, OUTPUT);
  pinMode(Bumpleft, INPUT); digitalWrite (Bumpleft, HIGH);
  pinMode(Bumpright, INPUT); digitalWrite (Bumpright, HIGH);
  pinMode(MOTOR1_A_PIN, OUTPUT);
  pinMode(MOTOR1_B_PIN, OUTPUT);

  pinMode(MOTOR2_A_PIN, OUTPUT);
  pinMode(MOTOR2_B_PIN, OUTPUT);
}

void forward() {
  digitalWrite(MOTOR1_A_PIN, LOW); 
  digitalWrite(MOTOR1_B_PIN, HIGH);
  digitalWrite(MOTOR2_A_PIN, LOW); 
  digitalWrite(MOTOR2_B_PIN, HIGH);
  analogWrite(MOTOR1_ENABLE_PIN, MAX_PWM_DUTY);
  analogWrite(MOTOR2_ENABLE_PIN, MAX_PWM_DUTY); // PWM 220/255 gives about 4.5 V at the motor

}

void reverse() {
  digitalWrite(MOTOR1_A_PIN, HIGH); 
  digitalWrite(MOTOR1_B_PIN, LOW);
  digitalWrite(MOTOR2_A_PIN, HIGH); 
  digitalWrite(MOTOR2_B_PIN, LOW);
  analogWrite(MOTOR1_ENABLE_PIN, MAX_PWM_DUTY);
  analogWrite(MOTOR2_ENABLE_PIN, MAX_PWM_DUTY);
}

void stop() {
  digitalWrite(MOTOR1_ENABLE_PIN, 0);
  digitalWrite(MOTOR2_ENABLE_PIN, 0);
}

void right() {
  digitalWrite(MOTOR1_A_PIN, LOW); 
  digitalWrite(MOTOR1_B_PIN, HIGH);
  digitalWrite(MOTOR2_A_PIN, LOW); 
  digitalWrite(MOTOR2_B_PIN, HIGH);

  analogWrite(MOTOR1_ENABLE_PIN, 0.8 * MAX_PWM_DUTY);
  analogWrite(MOTOR2_ENABLE_PIN, 1.0 * MAX_PWM_DUTY);
}

void spin_left() {
  digitalWrite(MOTOR1_A_PIN, LOW); 
  digitalWrite(MOTOR1_B_PIN, HIGH); // forward
  digitalWrite(MOTOR2_A_PIN, HIGH); 
  digitalWrite(MOTOR2_B_PIN, LOW); // reverse

  analogWrite(MOTOR1_ENABLE_PIN, 0.8 * MAX_PWM_DUTY);
  analogWrite(MOTOR2_ENABLE_PIN, 0.8 * MAX_PWM_DUTY);
}
  
  void spin_right() {
  digitalWrite(MOTOR2_A_PIN, LOW); 
  digitalWrite(MOTOR2_B_PIN, HIGH); // forward
  digitalWrite(MOTOR1_A_PIN, HIGH); 
  digitalWrite(MOTOR1_B_PIN, LOW); // reverse

  analogWrite(MOTOR2_ENABLE_PIN, 0.8 * MAX_PWM_DUTY);
  analogWrite(MOTOR1_ENABLE_PIN, 0.8 * MAX_PWM_DUTY); }


void loop()
{
  forward();
     if (!digitalRead(Bumpleft)) {
       reverse();
       delay(1000);
       spin_right();
       delay(750);
     }
     if (!digitalRead(Bumpright)) {
       reverse();
       delay(1000);
       spin_left();
       delay(750);
   }
   delay(50);
}




