// Include secrets and libraries
#include "secrets.h"
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

// ESP8266 D1 Mini Pin Definitions
#define D1 5

// Update variables
unsigned long lastUpdate = 0;
int updateTimeout = 60 * 1000; // 60s

// Create the DHT object on pin D1 using a DHT11 sensor
DHT dht(D1, DHT11);

// WiFi and ThingSpeak variables
char ssid[] = SECRET_SSID; 
char pass[] = SECRET_PASS;
WiFiClient  client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Update the ThingSpeak values
void update(){
  float temp = dht.readTemperature();
  float rH = dht.readHumidity();

  // Reread every 1000ms (1s) until both a valid temperature and valid rH are read
  while(isnan(rH) || isnan(temp)){
    delay(1000);
    temp = dht.readTemperature();
    rH = dht.readHumidity();
  }

  // Try to write the ThingSpeak fields while status is not 200
  int status;
  do{
    WiFiCheck();
    ThingSpeak.setField(1, (int)rH);
    ThingSpeak.setField(2, (int)temp);
    status = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    // Print error code if encountered
    if(status != 200){
      Serial.print("ThingSpeak error: ");
      Serial.println(status);
      delay(1000);
    }
  }while(status != 200);
}

// Ensure WiFi is still connected
void WiFiCheck(){
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("Connecting to ");
    Serial.print(SECRET_SSID);
    Serial.println("...");
    WiFi.begin(ssid, pass);
    delay(5000);
    if(WiFi.status() == WL_CONNECTED){
      Serial.println("Connected!");
    }
  }
}

void setup() {
  // Start serial for debugging
  Serial.begin(9600);

  // Start DHT object
  dht.begin();

  // Start WiFi and ThingSpeak
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  ThingSpeak.begin(client);

  // Initial update
  update();
}

void loop() {
  // run update at every interval. 
  if(millis() - lastUpdate >= updateTimeout){
    lastUpdate += updateTimeout;
    update();
  }
}