// 
// 
// 

#include "RTx.h"

RTx::RTx()
{
	this->inputCount = 0;
	this->outputCount = 0;

}

void RTx::reset() {
	this->inputCount = 0;
	this->outputCount = 0;
}

int RTx::send(int[]){
	// TODO: write data to writePin if isSerial == false
	// else write data to serial
}

String RTx::listen() {  
  Serial.println("started listening");
  for (int i=0; i<100; i++){
    if (Serial.available()) {
    String input = Serial.readStringUntil(';'); 
    Serial.println(input);
      return input ;
      break;
  }
 delay(1);
}
Serial.println("stopped listening");
}

