# SIT210-Task2.1PWebHook
S306 SIT210 Task 2.1P, WebHook to log temperature and humidity.

## Description
The pin definitions are for the ESP8266 based D1 Mini board. These definitions may be removed or modified for other boards

Connect the DHT11 temperature and humidity sensor to VCC, GND, and connect the DATA pin to pin D1. Every 30 seconds, the sketch will read from the sensor, then update the Arduino Cloud values. When these values are updated, the Zapier WebHook is triggered which then passes the latest values to a ThingSpeak channel. I also had to add a small capacitor to the power rail to stabilise power during WiFi activity.

## How it Works

## Usage