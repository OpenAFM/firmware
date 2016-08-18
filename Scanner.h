// Scanner.h
// Author A Michel
// Date 25/08/15
// Lego2nano 2015

/**
Definition of Scanner.
This class controls the scanning process and stores the pixel values.
It uses a controller to send commands to a TLC5620CN digital-to-analog chip and a SignalSampler
to collect pixel data. It uses an instance of RTx to send the data to the interface.

It also holds instance variables to keep track of the scanning time, scanning in progress and 
line count for information purposes.
**/
#ifndef _SCANNER_h
#define _SCANNER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "PiezoDACController.h"
#include "SignalSampler.h"
#include "RTx.h"


#define SCANNER_VERSION "0.0.1"

class Scanner
{

private:
	// DAC controller
	DACController controller;
	// Sampler
	SignalSampler sampler;
	// RTx (com)
	RTx phone;

	// number of lines scanned during the
	// scanning process.
	unsigned int linesScanned;

	// start time at Scanner.start()
	unsigned long startTime;

	// end time at Scanner.stop() or when the scanning finished.
	unsigned long endTime;

	// the length of the line (number of pixels)
	int lineLength;
	// pointer to the pixel array
	int *pixels;
	// indicates if scanning is in progress.
	bool scanning;
	// scans one line (trace and retrace) storing the data in the pixels array.
	int scanLine();


protected:

public:
	// Constructor
	Scanner(const DACController&, const SignalSampler&, const RTx&, const int lineLength);
	// destructor
	~Scanner();
	// resets all parameters. Should call controller.reset(), sampler.reset() and rtx.reset()
	void reset();
	
	// start scanning sequential scan
	int start();
	// stops the scanning processs
	int stop();
  // starts sending continous stream of data
  int stream();

	// update a param value
	void setParam(String param, String value);

	// get lapsed time
	unsigned long getLapsedTime();

	//
	void invertChannels();
};

#endif
