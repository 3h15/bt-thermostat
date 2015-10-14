#include <Wire.h>
#include "Adafruit_HDC1000.h"

struct SensorData {
  // Keep under 20 bytes!
  bool isValid; // 1 byte
  float temperature;  // 4 bytes
  float humidity;  // 4 bytes
  // NOTE: compiler will reserve a byte here not defined
};

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

  struct SensorData data = readFromHTC();

  if(data.isValid){
    Serial.println(data.temperature);
    Serial.println(data.humidity);
  }
  else{
    Serial.println("NO SENSOR?");
  }

  Serial.print("Loop Duration: ");
  Serial.println(millis() - loopStartTime);
  RFduino_ULPDelay(2000);
}


struct SensorData readFromHTC(){
  // Turn sensor on.
  digitalWrite(6, HIGH);

  hdc = Adafruit_HDC1000();

  struct SensorData result;
  if (hdc.begin()) {
    result.isValid = true;
    result.temperature = hdc.readTemperature();
    result.humidity = hdc.readHumidity();
  }
  else{
    result.isValid = false;
    result.temperature = 0;
    result.humidity = 0;
  }

  // Turn sensor off.
  digitalWrite(6, HIGH);

  return result;
}
