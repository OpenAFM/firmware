#include "PiezoDACController.h"
#include "DAC_AD5696.h"

#define ON HIGH
#define OFF LOW

// Channels available in chip.
extern const int CHANNEL_A = 0;
extern const int CHANNEL_B = 1;
extern const int CHANNEL_C = 2;
extern const int CHANNEL_D = 3;

// constructor
PiezoDACController::PiezoDACController(ADDAC *dac, int stepSize, int lineLength, int ldacPin, bool useRNG) {
	this->stepSize = stepSize;
	this->lineSize = lineLength;

	this->dac = dac;

	this->useRNG = useRNG;

	this->currentStep = 0;
	this->currentX = 0;
	this->currentY = 0;
	this->currentZ = 0;

	uint16_t half = 0x7FFF;
	startingXPlus = half;
	startingYPlus = half;
	startingXMinus = half;
	startingYMinus = half;

	currentXPlus = 0;
	currentYPlus = 0;
	currentXMinus = 0;
	currentYMinus = 0;

	invertChannels = false;
}

// destructor.
PiezoDACController::~PiezoDACController() {}

void PiezoDACController::Init()
{
	// should start with DACs at mid range
	SetDACOutput(AD569X_ADDR_DAC_ALL, 0x7FFF);
}

// reset parameters
unsigned int PiezoDACController::reset(int stepSize, int lineSize, int ldacPin, bool useRNG) {
  this->stepSize = stepSize;
  this->lineSize = lineSize;
  this->useRNG = useRNG;

  this->currentStep = 0;
  this->currentX = 0;
  this->currentY = 0;
  this->currentZ = 0;

  // should start with DACs at mid range
  SetDACOutput(AD569X_ADDR_DAC_ALL, 0x7FFF);

  invertChannels = false;

  return currentStep;
}


const byte mask = 128;

/*! Set the DAC output and update internal position. */
int PiezoDACController::SetDACOutput(uint8_t channels, uint16_t value)
{
	dac->SetOutput(channels, value);
	Serial.print("Setting DAC channel ");
	Serial.print(channels);
	Serial.print(" to ");
	Serial.println(value);

	if (channels & X_PLUS) currentXPlus = value;
	if (channels & X_MINUS) currentXMinus = value;
	if (channels & Y_PLUS) currentYPlus = value;
	if (channels & Y_MINUS) currentYMinus = value;


	return 0;
}

int PiezoDACController::move(PIEZO_DIRECTION direction, unsigned int steps, bool allAtOnce)
{
	int adiff = allAtOnce ? steps : 1; // if all at once, change voltage by full amount in one go.  Otherwise one at a time.
	int lim = allAtOnce ? 1 : steps;  // if all in one go, only do once, otherwise do each step

	uint8_t channelPlus = 0;
	uint8_t channelMinus = 0;
	uint16_t currentPlus = 0;
	uint16_t currentMinus = 0;
	int diff = 0;
	bool doSingle = true;


	// Decide what to move where
	switch (direction)
	{
		// for Z up/down, increment/decrement all dac channels
	case Z_UP:
		//currentZ++;
	case Z_DOWN:
		//currentZ--;
		diff = direction == Z_UP ? adiff : -adiff;  // increase or decrease?

		for (int i = 0; i < lim; i++)
		{
			SetDACOutput(X_PLUS, currentXPlus + diff);
			SetDACOutput(X_MINUS, currentXMinus + diff);
			SetDACOutput(Y_PLUS, currentYPlus + diff);
			SetDACOutput(Y_MINUS, currentYMinus + diff);
		}
		doSingle = false;
		break;

	case X_UP:
		//currentX++;
	case X_DOWN:
		//currentX--;
		diff = direction == X_UP ? adiff : -adiff;  // increase or decrease?
		currentPlus = currentXPlus;
		currentMinus = currentXMinus;
		channelPlus = X_PLUS;
		channelMinus = X_MINUS;
		break;

	case Y_UP:
		//currentY++;
	case Y_DOWN:
		//currentY--;
		diff = direction == Y_UP ? adiff : -adiff;  // increase or decrease?
		currentPlus = currentXPlus;
		currentMinus = currentXMinus;
		channelPlus = Y_PLUS;
		channelMinus = Y_MINUS;;
		break;

	}

	// change DAC
	if (doSingle)
	{
		for (int i = 0; i < lim; i++)
		{
			SetDACOutput(channelPlus, currentPlus + diff);
			SetDACOutput(channelMinus, currentMinus - diff);
		}
	}
	return 0;
}


int PiezoDACController::GotoCoordinates(uint16_t x, uint16_t y)
{
	// difference?
	int diffX = x - currentX;
	int diffY = y - currentY;

	move(diffX > 0 ? X_UP : X_DOWN, diffX, true);
	move(diffY > 0 ? Y_UP : Y_DOWN, diffY, true);
	currentX = x;
	currentY = y;
}

// reset coordinates to startingX and startingY
unsigned int PiezoDACController::reset() {
	currentStep = 0;
	//currentX = 0;
	//currentY = 0;
	currentZ = 0;
	//setCoordinates();
	GotoCoordinates(0, 0);
	//go(CHANNEL_A, currentX);
	//go(CHANNEL_B, currentY);
	return currentStep;
}

// move to beginning of next line.
unsigned int PiezoDACController::nextLine() {
	//int delta = (((currentStep / lineSize) + 1) * lineSize) - currentStep;
	//currentStep += delta;
	//setCoordinates();
	//go(CHANNEL_A, currentX);
	//go(CHANNEL_B, currentY);
	GotoCoordinates(0, currentY + 1);
	return currentStep;
}

// go to end of current line
unsigned int PiezoDACController::eol() {
	//nextLine();
	//currentStep--;
	//setCoordinates();
	//go(CHANNEL_A, currentX);
	//go(CHANNEL_B, currentY);
	GotoCoordinates(currentX, lineSize);
	return currentStep;
}

//// set coordinates relative to currentStep
//int PiezoDACController::setCoordinates() {
//	currentX = currentStep % lineSize;
//	currentY = currentStep / lineSize;
//
//	if (invertChannels) {
//		int temp = currentX;
//		currentX = currentY;
//		currentY = temp;
//	}
//	return 0;
//}

// increase voltage
unsigned int PiezoDACController::increaseVoltage() {

  // step fwd
  //currentStep += stepSize;
  
  //setCoordinates();
  
  move(X_UP, stepSize, false);
  //go(CHANNEL_A, currentX);
  //go(CHANNEL_B, currentY);
  
  return currentStep;
}

// decrease voltage
unsigned int PiezoDACController::decreaseVoltage() {

  // step back
  //currentStep -= stepSize;

  //setCoordinates();

  move(X_DOWN, stepSize, false);
  //go(CHANNEL_A, currentX);
  //go(CHANNEL_B, currentY);

  return currentStep;
}

// return current line size.
int PiezoDACController::getLineSize() {
  return lineSize;
}

// return current voltage for given channel
int PiezoDACController::getVoltage(int channel) {

  switch (channel) {

    case CHANNEL_A:
      return currentX;
    case CHANNEL_B:
      return currentY;
    case CHANNEL_C:
      return currentZ;

    default:
      return -1;
  }
}

void PiezoDACController::invert() {
  invertChannels = !invertChannels;
}



