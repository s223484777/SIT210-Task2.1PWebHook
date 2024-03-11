# SIT210-Task2.1PWebHook
S306 SIT210 Task 2.1P, WebHook to log temperature and humidity.

## Description
The pin definitions are for the ESP8266 based D1 Mini board. These definitions may be removed or modified for other boards

Connect the DHT11 temperature and humidity sensor to VCC, GND, and connect the DATA pin to pin D1. Every 30 seconds, the sketch will read from the sensor, then update the Arduino Cloud values. When these values are updated, the Zapier WebHook is triggered which then passes the latest values to a ThingSpeak channel. I also had to add a small capacitor to the power rail to stabilise power during WiFi activity.

## How it Works
The following code snippet calls the `readDHT()` function whenever the `updateTimeout` is reached.
```cpp
  if(millis() - lastUpdate >= updateTimeout){
    lastUpdate = millis();
    readDHT();
  }
```

The `readDHT()` function utilises a `do{...}while(...);` loop to try to read the sensor data until `result == 0`, meaning no errors were encounted and both values have been updated.
The loop has a 1000ms delay whenever a read fails to prevent the sensor from getting stuck in a timeout loop.

## Usage
The `arduino_secrets.h` file needs to contain the SSID (and password, if needed) for the WiFi network and the device key from Arduino Cloud. Additionally, two cloud variables will be defined on the Arduino Cloud with an update period of 30 seconds. We then need to add a WebHook (I have used Zapier) that is configured to call the ThingSpeak API with the updated variables.

The `thingProperties.h` file also needs to be updated with the correct `DEVICE_LOGIN_NAME[]`.

Set `updateTimeout` to the milliseconds between each update, for example 30000 for 30 seconds.