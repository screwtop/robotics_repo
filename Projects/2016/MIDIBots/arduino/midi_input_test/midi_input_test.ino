

void setup() {
	//  Set up serial port for MIDI baud rate (31.25 kbit/s):
	Serial.begin(31250);
	
	// We might want to use the LED to indicate activity (though don't use delay() unnecessarily (e.g. blink())).
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

  flash(); flash(); flash();
}

void loop() {
  int readByte;
  if (Serial.available() > 0) {
    readByte = Serial.read();
    if (readByte == 0x90)
      flash();
  }
}

void ledOn() {digitalWrite(LED_BUILTIN, HIGH);}

void ledOff() {digitalWrite(LED_BUILTIN, LOW);}

void flash() {
    ledOn();
    delay(50);
    ledOff();
}
