// 
// 
// 

#include "Controller.h"

#include <RunningMedian.h>

void ControllerClass::init(int stepSize, const int lineSize, int dataPin, int clkPin, 
	int loadPin, int ldacPin, bool rng, unsigned long serialRate,
	int samplerPin, int sampleSize)
{
	
	writer.init(stepSize, lineSize, dataPin, clkPin, loadPin, ldacPin, rng, serialRate);
	sampler.init(samplerPin, sampleSize);

	this->totalSteps = lineSize *  lineSize - 1;
	this->lineSize = lineSize;
	this->sampleSize = sampleSize;
}


int ControllerClass::scanLine() {

	RunningMedian samples = RunningMedian(sampleSize);

	int* trace = 0;
	if (trace != 0) {
		delete[] trace;
	}
	trace = new int[lineSize];

	for (unsigned int x = 0; x < writer.getLineSize(); x++) {
		int median = sampler.detectPixel(samples);
		trace[x] = median;

		// don't move beyond eol
		if (x + 1 < writer.getLineSize())
			writer.fwd();
	}

	// TODO send data

	// clean array for re use
	if (trace != 0) {
		delete[] trace;
	}
	trace = new int[lineSize];

	for (unsigned int x = 0; x < writer.getLineSize(); x++) {
		int median = sampler.detectPixel(samples);
		trace[x] = median;

		// don't move beyond the BOL
		if (x - 1 > 0)
			writer.bwd();
	}
}



ControllerClass Controller;

