#include "Scanner.h"
#include "RTx.h"

/* Pin Definitions */

//Pins for DAC (tlc5620)
#define _SDI 12 //Data output pin (this number will be converted to analog)(MSB first)
#define _SCK 11 //Clock
#define LOAD 9  //Load HIGH to clock data into DAC
#define LDAC 10 //ldac HIGH to stop DAC output while clocking. 
#define RNG 0   //HIGH doubles the output voltage

//Imaging parameters
#define STEPSIZE 1      //Stepsize of DAC(linelength/stepsize=number of pixel per line)
#define LINE_LENGTH 10 //Voltage range over (MAX256). Defines size of image in real space
#define SAMPLE_SIZE 5   //Number of samples taken at each pixel. Median is taken as true value

//Communication parameters
#define BAUDRATE 115200   //Serial interfaces communication speed (bps)
#define SAMPLER_PIN_A A0  //Input A
#define SAMPLER_PIN_B A1  //Input B 

/* Setup */

RTx* phone=new RTx();
DACController* ctrl=new DACController(STEPSIZE, LINE_LENGTH, _SDI, _SCK, LOAD, LDAC, RNG);
SignalSampler* sampler=new SignalSampler(SAMPLER_PIN_A, SAMPLER_PIN_B, SAMPLE_SIZE);
Scanner* scanner=new Scanner(*ctrl, *sampler, *phone, LINE_LENGTH);

//This function runs once, when the arduino starts
void setup() {
   
  Serial.begin(BAUDRATE);
}

extern String const PARAM_LINE_LENGTH;

//This function keeps looping
void loop() {

  String cmd = phone->listen();
  
  delay(1);
  
  if (cmd=="GO") {
    scanner->start();
  }

  if (cmd == "SETUP") {
      delete ctrl;
      delete sampler;
      delete scanner;
      
      int CUSTOM_STEPSIZE=Serial.parseInt();
      int CUSTOM_LINE_LENGTH=Serial.parseInt();
      int CUSTOM_SAMPLE_SIZE=Serial.parseInt();
     
      ctrl =new DACController(CUSTOM_STEPSIZE, CUSTOM_LINE_LENGTH, _SDI, _SCK, LOAD, LDAC, RNG);
      sampler = new SignalSampler(SAMPLER_PIN_A, SAMPLER_PIN_B, CUSTOM_SAMPLE_SIZE);
      scanner = new Scanner(*ctrl, *sampler, *phone, CUSTOM_LINE_LENGTH);     
    
  }
  if (cmd == "STREAM")
  {
    scanner->stream();
  }

  else if (cmd="ERROR"){;}
  
}
