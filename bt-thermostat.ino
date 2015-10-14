#include <Wire.h>
#include "Adafruit_HDC1000.h"
#include <RFduinoGZLL.h>

struct SensorData {
  // Keep under 20 bytes!
  bool isValid; // 1 byte
  float temperature;  // 4 bytes
  float humidity;  // 4 bytes
  // NOTE: compiler will reserve a byte here not defined
};

device_t role = DEVICE0;
bool isSending = false;

Adafruit_HDC1000 hdc;

void setup(){

  Serial.begin(9600);

  // Gnd on pin 4 for HTC sensor
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  // Vcc on pin 3 for HTC sensor
  pinMode(6, OUTPUT);

  // Max power
  RFduinoGZLL.txPowerLevel = +4;

  // Change addresses to avoid conflicts with other networks.
  // (note: the msb cannot be 0x55 or 0xaa)
  RFduinoGZLL.hostBaseAddress = 0x4A27E98F;
  RFduinoGZLL.deviceBaseAddress = 0xF487EE4A;



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

  sendToHost(data);

  Serial.print("Loop Duration: ");
  Serial.println(millis() - loopStartTime);
  RFduino_ULPDelay(2000);
}


void sendToHost(struct SensorData data){
  // Start GZLL
  RFduinoGZLL.begin(role);

  // Send data
  isSending = true;
  RFduinoGZLL.sendToHost((char *)&data, sizeof(data));

  // Wait max 10 seconds for ACK
  int startTime = millis();
  while(isSending && ( millis()-startTime < 10000)){
    delay(10);
  }

  // Stop GZLL
  RFduinoGZLL.end();
}

void RFduinoGZLL_onReceive(device_t device, int rssi, char *data, int len)
{
  // Reset sending state.
  isSending = false;
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
