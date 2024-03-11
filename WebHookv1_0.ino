#include "arduino_secrets.h"
// ESP8266 D1 Mini Pin Definitions
#define D1    5

// Include the Arduino Cloud header file and DHT11 library
#include "thingProperties.h"
#include <DHT11.h>

// Update variables
unsigned long lastUpdate = 0;
int updateTimeout = 30000; // 30s

// Create the DHT11 object
DHT11 dht(D1);

// Read the DHT11 sensor (blocking)
void readDHT(){
  int result = 0;
  do{
    int tmp_t, tmp_r; // Temporary values while reading sensor
    result = dht.readTemperatureHumidity(tmp_t, tmp_r);

    if(result != 0){
      // Delay successive reads to prevent repeated timeouts of the sensor
      delay(1000);
    }else{
      // Update the Arduino Cloud variables on a successful read
      rH = tmp_r;
      temperature = tmp_t;
    }
  
  // Loop until a successful read
  }while(result != 0);
}

void setup() {
  Serial.begin(9600);
  delay(1500);
  
  // Take an initial reading before conneccting to the cloud
  readDHT();

  // Initialise and set up Arduino Cloud
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  ArduinoCloud.update();
}

void loop() {
  if(millis() - lastUpdate >= updateTimeout){
    lastUpdate = millis();
    readDHT();
  }
  ArduinoCloud.update();
}