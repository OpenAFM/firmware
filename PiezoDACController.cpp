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
PiezoDACController::PiezoDACController(int stepSize, int lineLength, int ldacPin, bool useRNG) {
  this->stepSize = stepSize;
  this->lineSize = lineLength;

  this->ldacPin = ldacPin;
  this->useRNG = useRNG;

  this->currentStep = 0;
  this->currentX = 0;
  this->currentY = 0;
  this->currentZ = 0;

  // make the DAC
  DAC_AD5696 daq;

  pinMode(this->ldacPin, OUTPUT);

  digitalWrite(this->ldacPin, OFF);

  invertChannels = false;
}

// destructor.
PiezoDACController::~PiezoDACController() {}

// reset parameters
unsigned int PiezoDACController::reset(int stepSize, int lineSize, int ldacPin, bool useRNG) {
  this->stepSize = stepSize;
  this->lineSize = lineSize;

  this->ldacPin = ldacPin;
  this->useRNG = useRNG;

  this->currentStep = 0;
  this->currentX = 0;
  this->currentY = 0;
  this->currentZ = 0;

  pinMode(this->ldacPin, OUTPUT);

  digitalWrite(this->ldacPin, OFF);


  invertChannels = false;

  return currentStep;
}


const byte mask = 128;


// set voltage value for given channel
int PiezoDACController::go(int channel, int value) {
  
  //  Set DAC Channel LMB (left most significant bit)
  // 0 0 = A  X
  // 0 1 = B  Y
  // 1 0 = C  Z
  // 1 1 = D  -
  
  switch (channel)
  {
    case CHANNEL_A:
      setBitOff();
      setBitOff();
      break;

    case CHANNEL_B:
      setBitOff();
      setBitOn();
      break;

    case CHANNEL_C:
      setBitOff();
      setBitOn();
      break;

    case CHANNEL_D:
      setBitOn();
      setBitOff();
      break;
  }

  // 1x gain (0) or 2x (1)
  useRNG ? setBitOn() : setBitOff();

  /*
  for (int i = 7; i >= 0; i--)
  {
    // shifting bits to the right.
    int k = value >> i;

    // true if k (LMS) is 1
    if (k & 1) {
      sendHighSignal();
    } else {
      sendLowSignal();
    }
  }
  */
  for (int i = 0; i <= 7; i++) {
    if (value & mask) {
      setBitOn();
    } else {
      setBitOff();
    }
    value = value << 1;
  }
   unsigned int t = micros();
    while (micros() - t <= 10)
  {
    // Wait...
  }      
  
    // load to output registers
  loadDAC();
}

// send high
int PiezoDACController::setBitOn() {
	digitalWrite(clockPin, ON);
	digitalWrite(dataPin, ON);
	digitalWrite(clockPin, OFF);
}
// send low
int PiezoDACController::setBitOff() {
	digitalWrite(clockPin, ON);
	digitalWrite(dataPin, OFF);
	digitalWrite(clockPin, OFF);
}

// load dac
int PiezoDACController::loadDAC() {
  digitalWrite(loadPin, OFF);
  digitalWrite(loadPin, ON);
}

// reset coordinates to 0,0
unsigned int PiezoDACController::reset() {
  currentStep = 0;
  currentZ = 0;
  setCoordinates();
  go(CHANNEL_A, currentX);
  go(CHANNEL_B, currentY);
  return currentStep;
}

// move to beginning of next line.
unsigned int PiezoDACController::nextLine() {
  int delta = (((currentStep / lineSize) + 1) * lineSize) - currentStep;
  currentStep += delta;
  setCoordinates();
  go(CHANNEL_A, currentX);
  go(CHANNEL_B, currentY);
  return currentStep;
}

// go to end of current line
unsigned int PiezoDACController::eol() {
  nextLine();
  currentStep--;
  setCoordinates();
  go(CHANNEL_A, currentX);
  go(CHANNEL_B, currentY);
  return currentStep;
}

// set coordinates relative to currentStep
int PiezoDACController::setCoordinates() {
  currentX = currentStep % lineSize;
  currentY = currentStep / lineSize;

  if (invertChannels) {
    int temp = currentX;
    currentX = currentY;
    currentY = temp;
  }
}

// increase voltage
unsigned int PiezoDACController::increaseVoltage() {

  // step fwd
  currentStep += stepSize;
  
  setCoordinates();
  
  go(CHANNEL_A, currentX);
  go(CHANNEL_B, currentY);
  
  return currentStep;
}

// decrease voltage
unsigned int PiezoDACController::decreaseVoltage() {

  // step back
  currentStep -= stepSize;

  setCoordinates();

  go(CHANNEL_A, currentX);
  go(CHANNEL_B, currentY);

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



