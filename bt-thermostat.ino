#include <Wire.h>
#include "Adafruit_HDC1000.h"


Adafruit_HDC1000 hdc;

void setup(){

  Serial.begin(9600);

  Serial.println("START");

  // Vcc on pin 3 for HTC sensor
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);

  // Gnd on pin 4 for HTC sensor
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  hdc = Adafruit_HDC1000();

  if (!hdc.begin()) {
    Serial.println("NO SENSOR?");
    while (1);
  }
}

void loop() {
  Serial.print("T: ");
  Serial.print(hdc.readTemperature());
  Serial.print("\t\tH: ");
  Serial.println(hdc.readHumidity());
  delay(2000);
}
