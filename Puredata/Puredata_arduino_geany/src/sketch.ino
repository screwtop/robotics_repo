const int Reflect = 16;
void setup()
{
	Serial.begin(115200);
}

void loop()
{
	Serial.write(analogRead (Reflect));
	delay(15);
}
