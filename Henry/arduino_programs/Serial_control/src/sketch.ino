const int Switch1Pin = 2;
const int Switch2Pin = 3;
int Range_Threshold = 50; //cm
const int Ledpin = 8;
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
int serdata;
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
	//Serial.println("Arduino listening...");
	Serial.println("?");
}

void forwards(int PWM)  {
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
  digitalWrite(MOTOR_R_A_PIN, LOW);
  digitalWrite(MOTOR_R_B_PIN, HIGH); // forward
  digitalWrite(MOTOR_L_A_PIN, LOW);
  digitalWrite(MOTOR_L_B_PIN, HIGH); // reverse

  analogWrite(MOTOR_R_ENABLE_PIN, 0.8 * SLOW_PWM_DUTY);
  analogWrite(MOTOR_L_ENABLE_PIN, 0.8 * SLOW_PWM_DUTY);
}

void left() {
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
void loop(){
	serial_control();
}
void serial_control(){ 	
if (Serial.available() > 0)
  {
	  String mode  = Serial.readStringUntil(',');
    Serial.read(); //next character is comma, so skip it using this
    String dir = Serial.readStringUntil('\0');
    Serial.println("mode:");
    Serial.println(mode);
    Serial.println("time");
    Serial.println(dir);
	if(mode == "B")
    {
	reverse(MAX_PWM_DUTY);
    delay(dir.toInt());
	stop();
	
	
	}
	if(mode == "F")
    {
	forwards(MAX_PWM_DUTY);
    delay(dir.toInt());
	stop();
	
	}
	
	if(mode == "L")
    {
	left();
    delay(dir.toInt());
	
	stop();
	}
	if(mode == "R")
    {
	right();
    delay(dir.toInt());
	stop();
	
}
	if(mode == "SL")
    {
	spin_left();
    delay(dir.toInt());
	stop();
	
	}
	if(mode == "SR")
    {
	spin_right();
    delay(dir.toInt());
	stop();
	
	}
	if(mode == "Q")
    {
	brake();
	}
	
	if(mode == "H")   {
		_help();
	}
}
}	

void _help(){
 Serial.println("HELP:\n");
		Serial.println("Usage:");
		Serial.println("[mode],[duriation in m\s]");
		Serial.println("Commands:");
		Serial.println("F = forwards");
		Serial.println("B = reverse");
		Serial.println("R = right");
		Serial.println("L = left");
		Serial.println("SR = spin right");
		Serial.println("SL = spin left");
}
