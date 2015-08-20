
// Pin definitions
int SDI = 2;
int _SCK = 3;
int LOAD = 4;
int LDAC = 5;

int incomingByte;
int level;

// the setup routine runs once when you press reset:
void setup() {

	Serial.begin(115200);

	// initialize output pins
	pinMode(SDI, OUTPUT);
	pinMode(_SCK, OUTPUT);
	pinMode(LOAD, OUTPUT);
	pinMode(LDAC, OUTPUT);

	// activate DAC
	digitalWrite(LOAD, HIGH);
	digitalWrite(LDAC, LOW);

	// command is clocked on rising edge
	digitalWrite(_SCK, LOW);
}

void writeHigh(){
	digitalWrite(_SCK, HIGH);
	digitalWrite(SDI, HIGH);
	digitalWrite(_SCK, LOW);
}

void writeLow(){
	digitalWrite(_SCK, HIGH);
	digitalWrite(SDI, LOW);
	digitalWrite(_SCK, LOW);
}

void loadDAC(){
	digitalWrite(LOAD, LOW);
	digitalWrite(LOAD, HIGH);
}

// channel max is 4095
void DACwrite(int input, int channel) {

	int k;

	// range from 0 to 255
	input = input % 256;

	// choose channel
	// 0 0 = A
	// 1 0 = B
	// 0 1 = C
	// 1 1 = D
	writeHigh();
	writeHigh();

	// 1x gain (0) or 2x (1)
	writeLow();
	// bit shifting to determine when to 
	// send 1 and when to send 0 in the command 
	// word.
	// sending most significant bit first
	// TODO negation ~ ??
 
	for (int c = 7; c >= 0; c--)
	{
		// 0110 - 6
		// -----
		// 0000 - 0 - 0
		// 0001 - 1 - 1
		// 0011 - 3 - ??
		// shifting bits to the right.
		k = input >> c;
    //writeHigh();
		// true if k is 1
		if (k & 1){
			// this is a 1
			writeHigh();
		}
		else{
			// this is a 0
			writeLow();
		}

	}

	// load to output registers
	loadDAC();
}

// the loop routine runs over and over again forever:
void loop() {
  for (int i = 0; i <= 254; i++)
  {
	DACwrite(i, 0);
	delay(20);
  }

}
