#include <Wire.h>
#include "Adafruit_HDC1000.h"


Adafruit_HDC1000 hdc;

void setup(){

  Serial.begin(9600);

  // Gnd on pin 4 for HTC sensor
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  // Vcc on pin 3 for HTC sensor
  pinMode(6, OUTPUT);

}

void loop() {
  int loopStartTime = millis();

  // Turn sensor on.
  digitalWrite(6, HIGH);

  hdc = Adafruit_HDC1000();

  if (hdc.begin()) {
    Serial.println(hdc.readTemperature());
    Serial.println(hdc.readHumidity());
  }

  // Turn sensor off.
  digitalWrite(6, HIGH);

  Serial.print("Loop Duration: ");
  Serial.println(millis() - loopStartTime);
  RFduino_ULPDelay(2000);
}
