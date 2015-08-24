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

int RTx::sendData(int pixels[], int pixelCount) {
  for (int i = 0; i < pixelCount; ++i) {
    Serial.print(pixels[i]);
    if (i == pixelCount - 1)
      Serial.println(';'); // Send ';' when all numbers are sent
    else
      Serial.print(','); // Send ',' when a number is sent
  }
}


String RTx::listen() {  
  Serial.println("started listening");
  for (int i=0; i<100; i++){
    if (Serial.available()) {
    delay(40); //wait for entire serial to arrive
    String input = Serial.readStringUntil(';'); //stop at ;
    Serial.println(input);
      return input ;
      break;
  }
 delay(1);
}
Serial.println("stopped listening");
}

