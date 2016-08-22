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

  this->useRNG = useRNG;

  this->currentStep = 0;
  this->currentX = 0;
  this->currentY = 0;
  this->currentZ = 0;

  invertChannels = false;
}

// destructor.
PiezoDACController::~PiezoDACController() {}

// reset parameters
unsigned int PiezoDACController::reset(int stepSize, int lineSize, int ldacPin, bool useRNG) {
  this->stepSize = stepSize;
  this->lineSize = lineSize;
  this->useRNG = useRNG;

  this->currentStep = 0;
  this->currentX = 0;
  this->currentY = 0;
  this->currentZ = 0;


  invertChannels = false;

  return currentStep;
}


const byte mask = 128;


int PiezoDACController::move(PIEZO_DIRECTION direction, unsigned int times)
{

}

// reset coordinates to 0,0
unsigned int PiezoDACController::reset() {
  currentStep = 0;
  currentZ = 0;
  setCoordinates();
  //go(CHANNEL_A, currentX);
  //go(CHANNEL_B, currentY);
  return currentStep;
}

// move to beginning of next line.
unsigned int PiezoDACController::nextLine() {
  int delta = (((currentStep / lineSize) + 1) * lineSize) - currentStep;
  currentStep += delta;
  setCoordinates();
  //go(CHANNEL_A, currentX);
  //go(CHANNEL_B, currentY);
  return currentStep;
}

// go to end of current line
unsigned int PiezoDACController::eol() {
  nextLine();
  currentStep--;
  setCoordinates();
  //go(CHANNEL_A, currentX);
  //go(CHANNEL_B, currentY);
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
  
  //go(CHANNEL_A, currentX);
  //go(CHANNEL_B, currentY);
  
  return currentStep;
}

// decrease voltage
unsigned int PiezoDACController::decreaseVoltage() {

  // step back
  currentStep -= stepSize;

  setCoordinates();

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



