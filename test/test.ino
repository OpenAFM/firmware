#inculde "DataRecv.h"

#define DataRecvPin A7
#define detectTimes 10 // Samples of one pixel

void setup() {
        Serial.begin(9600);
        pinMode(DataRecvPin, INPUT);
}

void loop() {
        int pixelData[pixelCount];    
        for (int i = 0; i < pixelCount; ++i) {
                pixelData[i] = DataRecv.detectPixel(DataRecvPin);
                Serial.println(pixelData[i]);
        }
}

