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

void RTx::sendData(int pixels[], int pixelCount){
  
  while (true){
  String cmd=listen();
  if (cmd=="RDY"){break;}
  else if (cmd="ERROR"){break;}
  }
  
  for (int i = 0; i < pixelCount; ++i) {
    Serial.print(pixels[i]);
    if (i == pixelCount - 1)
      Serial.println(';'); // Send ';' when all numbers are sent
    else
      Serial.print(','); // Send ',' when a number is sent
  }
}

void RTx::sendString(String message){
  Serial.print(message);
  Serial.println(';'); // Send ';' when all numbers are sent
}


String RTx::listen() {
  String retVal = ""; 
  for (int i=0; i<100; i++){
    if (Serial.available()) {
    delay(40); //wait for entire serial to arrive
    String returnVal = Serial.readStringUntil(';'); //stop at ;
    Serial.println(returnVal);
      break;
  }
  return retVal;
 delay(1);
}
}

