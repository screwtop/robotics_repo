  // MIDI output testing, CME 2016


// Tempo:
const int del = 100;
//const int del = 200;
//const int del = 60;

//const int switchPin = 2;
//int switchState = 0;
const int ledPin = 13;

// Arrays for the current and previous state of all the key switches:
// Notably, we need to keep the TX pin free for MIDI transmission.
// NOTE: on the Arduino Mega, the big double-row of pins starts has a pair of 5V pins, then digital pins 22 and 23.  The last digital pin is 53, and then there are two grounds at the end.
const int numKeys = 56;
const int firstKeyPin = 2;
// NOTE: awkward mismatch of pin numbers and array indexes, since we're not necessarily starting at 0!  It's a bit wasteful of memory, but let's just make the array bigger, and ignore any we aren't using, and use the same numbering for the array elements as for the pin numbers for the input switches.
int currSwitchState[numKeys + firstKeyPin];
int prevSwitchState[numKeys + firstKeyPin];

void setup() {
	//  Set up serial port for MIDI baud rate (31.25 kbit/s):
	Serial.begin(31250);
//	Serial.begin(9600);
//	Serial.println("setup()...");
	
//	pinMode(switchPin, INPUT); digitalWrite(switchPin, HIGH);	// Microswitch input, needs internal pull-up resistor.
	pinMode(ledPin, OUTPUT); digitalWrite(ledPin, LOW);

	// Set up input pins and initialise switch state arrays:
	for (int pin = firstKeyPin; pin <= firstKeyPin + numKeys - 1; pin++) {
	//	Serial.println(pin);
		pinMode(pin, INPUT);
		digitalWrite(pin, HIGH);
		currSwitchState[pin] = 0;
		prevSwitchState[pin] = 0;
	}

}

void loop() {
	readSwitches();
//	switchTest(); delay(2);
//	glissando();
//	playRandomNote();
//	discoBeat();
//	hecticBeat();
//	playArpeggios();
}
void glissando() {
	// play notes from F#-0 (0x1E) to F#-5 (0x5A):
	for (int note = 0; note < 128; note ++) {
		// Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
		playNote(1, note, 100);
		delay(50);
		// Note on channel 1 (0x90), same note value (note), silent velocity (0x00):
		playNote(1, note, 0x00);
	//	delay(100);
	}
}

// This function from example Arduino code:
//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int cmd, int pitch, int velocity) {
	Serial.write(cmd);
	Serial.write(pitch);
	Serial.write(velocity);
}

// IMO, a more sensible way would be to construct the first byte based on the channel number, and not require the 0x9 to be passed at all:
// NOTE: channel numbers here are the logical user-friendly channel numbers (1..16), not the internal values used to represent them.
void playNote(int channel, int pitch, int velocity) {
	// Convert channel to a 4-bit value:
	if (channel > 16 || channel < 1) channel = 0;
	Serial.write(0x90 | channel);
	Serial.write(pitch);
	Serial.write(velocity);
}

void playRandomNote() {
	int pitch = random(30, 110);
	int velocity = random(40, 120);
	int duration = random(20, 100);
	int pause = random(0, 100);
	noteOn(0x90, pitch, velocity);
	delay(duration);
	noteOn(0x90, pitch, 0); // off
//	delay(pause);
}

void playArpeggios() {
    int octave = random(0,4) + 4;
    int arpDegree = random(0,3);
    int note = octave * 12 + scaleDegreeToSemitone(arpeggioDegreeToScaleDegree(arpDegree));
    playNote(1, note, 100);
    delay(70);
    playNote(1, note, 0);
}


int scaleDegreeToSemitone(int scaleDegree) {
	if (scaleDegree == 0) return 0;
	else if (scaleDegree == 1) return 2;
	else if (scaleDegree == 2) return 4;
	else if (scaleDegree == 3) return 5;
	else if (scaleDegree == 4) return 7;
	else if (scaleDegree == 5) return 9;
	else if (scaleDegree == 6) return 11;
	else return 0;
}

int arpeggioDegreeToScaleDegree(int arpeggioDegree) {
	if (arpeggioDegree == 0) return 0;
	else if (arpeggioDegree == 1) return 3;
	else if (arpeggioDegree == 2) return 5;
	else return 0;
}


void discoBeat() {
	int velocity = random(64, 128);
	playNote(10, 36, velocity); delay(del); playNote(10, 36, 0);
	playNote(10, 42, velocity); delay(del); playNote(10, 42, 0);
	playNote(10, 38, velocity); delay(del); playNote(10, 38, 0);
	playNote(10, 42, velocity); delay(del); playNote(10, 42, 0);
}

void hecticBeat() {
	int velocity = random(48, 128);
	playNote(10, 36, velocity); delay(del); playNote(10, 36, 0);
	playNote(10, 41, velocity); delay(del); playNote(10, 41, 0);
	playNote(10, 38, velocity); delay(del); playNote(10, 38, 0);
	playNote(10, 41, velocity); delay(del); playNote(10, 41, 0);
}

// Trigger a MIDI note based on an attached switch.  We'll use polling for this (so it can be scaled up to a polyphonic keyboard controller), and we'll also need to remember the state of the switch(es) for triggering note on/off.
/*
void switchTest() {
	if (!digitalRead(switchPin) != switchState) {
		switchState = !switchState;
		playNote(1, 69, switchState * 100);
	}
	digitalWrite(ledPin, switchState);
}
*/

void blink() {
	digitalWrite(ledPin, HIGH);
	delay(50);
	digitalWrite(ledPin, LOW);
}

// Read the state of the input key switches and trigger note on/off MIDI events as necessary.
void readSwitches() {
	for (int pin = firstKeyPin; pin <= firstKeyPin + numKeys - 1; pin++) {
		currSwitchState[pin] = !digitalRead(pin);	// Active-low; negate here.
	//	digitalWrite(ledPin, currSwitchState[pin]);
		if (currSwitchState[pin] != prevSwitchState[pin]) {
		//	blink();
		//	playNote(1, 60 + pin, 60);
			playNote(1, 36 + pin, currSwitchState[pin] * 100);	// Note on/off depending on whether the key is pressed.
		}
		prevSwitchState[pin] = currSwitchState[pin];	// Store the state for next time.
	}
	delay(1); // Hold off just a little do reduce spurious note on/off due to switch contact bouncing.
}

