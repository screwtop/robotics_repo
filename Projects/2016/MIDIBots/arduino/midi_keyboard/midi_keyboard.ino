// A simple MIDI keyboard for Arduino
// CME 2016-03

// This polls for input switch states and outputs MIDI note on/off messages when they change, i.e. when a switch is pressed/released.
// We reserve a block of contiguous input pins to keep the code simple and scalable.

// To monitor MIDI messages:
// aseqdump -p 20:0

// To play via Yoshimi:
// yoshimi --alsa-midi=20:0 --alsa-audio=pulse --load-instrument=/usr/share/yoshimi/banks/Will_Godfrey_Collection/0033-Medium\ Saw.xiz


// Here we define a couple of arrays for the current and previous state of all the key switches:
// Notably, we need to keep the TX pin free for MIDI transmission. Also the LED_BUILTIN pin so we can indicate activity.
// NOTE: on the Arduino Mega, the big double-row of pins starts has a pair of 5V pins, then digital pins 22 and 23.  The last digital pin is 53, and then there are two grounds at the end.

const int NUM_KEYS = 32;
const int FIRST_KEY_PIN = 22;

// Here are the arrays.
// NOTE: awkward mismatch of pin numbers and array indexes, since we're not necessarily starting at 0!  It's a bit wasteful of memory, but let's just make the array bigger, and ignore any we aren't using, and use the same numbering for the array elements as for the pin numbers for the input switches.

int currSwitchState[NUM_KEYS + FIRST_KEY_PIN];
int prevSwitchState[NUM_KEYS + FIRST_KEY_PIN];


void setup() {
	//  Set up serial port for MIDI baud rate (31.25 kbit/s):
	Serial.begin(31250);
	
	// We might want to use the LED to indicate activity (though don't use delay() unnecessarily (e.g. blink())).
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	// Set up input pins and initialise switch state arrays:
	for (int pin = FIRST_KEY_PIN; pin <= FIRST_KEY_PIN + NUM_KEYS - 1; pin++) {
		pinMode(pin, INPUT_PULLUP);
		currSwitchState[pin] = 0;
		prevSwitchState[pin] = 0;
	}

}

void loop() {
	readSwitches();
}

void ledOn() {digitalWrite(LED_BUILTIN, HIGH);}

void ledOff() {digitalWrite(LED_BUILTIN, LOW);}

// This function is used to conveniently generate note on/off MIDI messages.
// The first message byte is called the status byte, and indicates the type of message and the channel number being addressed.
// Status bytes have a 1 for the MSB, and data bytes always have a 0 there.
// 0x9 (0b1001) is the Note On status nybble.
// NOTE: the channel number parameter should be the logical (user-centric) channel numbers (1..16), not the internal values used to represent them.

void playNote(int channel, int pitch, int velocity) {
	if (channel > 16 || channel < 1) channel = 1;	// Clip to valid range
	Serial.write(0x90 | (channel-1));	// write status byte, converting channel number to wire format
	Serial.write(pitch);			// data byte 1
	Serial.write(velocity);			// data byte 2
}


// Trigger a MIDI note based input switch activity.  We'll use polling for this (there wouldn't be enough interrupts available for an interrupt-based approach). As a result, we need to remember the state of the switch(es) from the last poll so we can recognise on/off transitions.

void readSwitches() {
	// Check each of the pins:
	for (int pin = FIRST_KEY_PIN; pin <= FIRST_KEY_PIN + NUM_KEYS - 1; pin++) {
		currSwitchState[pin] = !digitalRead(pin);	// Active-low; negate here.
		// If state changed, output note on/off:
		if (currSwitchState[pin] != prevSwitchState[pin]) {
			ledOn();
			playNote(1, 36 + pin, currSwitchState[pin] * 100);	// Note on/off depending on whether the key is pressed.
			ledOff();
		}
		prevSwitchState[pin] = currSwitchState[pin];	// Store the state for next time.
	}
	delay(1); // Hold off just a little do reduce spurious note on/off due to switch contact bouncing.
}

