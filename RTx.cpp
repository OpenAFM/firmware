#include "RTx.h"

RTx::RTx() {
	this->inputCount = 0;
	this->outputCount = 0;
}

void RTx::reset() {
	this->inputCount = 0;
	this->outputCount = 0;
}

int RTx::sendData(int pixels[], int pixelCount){
  String cmd;
  while (true) {
	cmd = listen();
	if (cmd == "RDY" || cmd == "DONE"){break;}
  }

  //sending data
  for (int i = 0; i < pixelCount; ++i) {
    Serial.print(pixels[i]);
    if (i == pixelCount - 1){
      Serial.print(';'); // Send ';' when all numbers are sent
	} else
      Serial.print(','); // Send ',' when a number is sent
  }

  Serial.flush();

  if (cmd=="DONE") return 0;

  else return 1;
}

//only send message if other side is ready for it
void RTx::sendString(String message){
  String cmd;
  while (true){
  cmd=listen();
  if (cmd=="RDY"){break;}
  else if (cmd=="DONE"){break;}
  }
  Serial.print(message);
  Serial.println(';'); // Send ';' when all numbers are sent
}


String RTx::listen() {
  String retVal = ""; 
  for (int i=0; i<100; i++){
    if (Serial.available()) {
    delay(40); //wait for entire serial to arrive
    retVal = Serial.readStringUntil(';'); //stop at ;
    //return input value
    Serial.print(retVal); 
    Serial.print(';');
    Serial.flush();
      break;
  }
  return retVal;
 delay(1);
}
}

