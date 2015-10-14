#include <Wire.h>
#include "Adafruit_HDC1000.h"
#include <RFduinoGZLL.h>

struct SensorData {
  // Keep under 20 bytes!
  bool isValid; // 1 byte
  float temperature;  // 4 bytes
  float humidity;  // 4 bytes
  char id; // 1 byte
  // NOTE: compiler will reserve a byte here not defined
};

device_t role = DEVICE0;
bool isSending = false;

Adafruit_HDC1000 hdc;

void setup(){
  // Start by sleeping for a while. This is the only way to be sure the board is really in ULP mode...
  RFduino_ULPDelay(10000);
}

void loop() {
  // Do the work
  sendToHost(readFromHTC());
  // Go back to ULP mode
  RFduino_systemReset();
}


void sendToHost(struct SensorData data){

  // Max power
  RFduinoGZLL.txPowerLevel = +4;

  // Change addresses to avoid conflicts with other networks.
  // (note: the msb cannot be 0x55 or 0xaa)
  RFduinoGZLL.hostBaseAddress = 0x4A27E98F;
  RFduinoGZLL.deviceBaseAddress = 0xF487EE4A;

  // Start GZLL
  RFduinoGZLL.begin(role);

  // Send data
  isSending = true;
  RFduinoGZLL.sendToHost((char *)&data, sizeof(data));

  // Wait max 5 seconds for ACK
  int startTime = millis();
  while(isSending && ( millis()-startTime < 5000)){
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
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW); // Gnd
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH); // Vcc

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

  result.id = 'A';

  return result;
}
