# SIT210-Task2.1PWebHook
S306 SIT210 Task 2.1P, WebHook to log temperature and humidity.

## Description
The pin definitions are for the ESP8266 based D1 Mini board. These definitions may be removed or modified for other boards

Connect the DHT11 temperature and humidity sensor to VCC, GND, and connect the DATA pin to pin D1. Every 60 seconds, the sketch will read from the sensor, and push the readings to a ThingSpeak channel. I also had to add a small capacitor to the power rail to stabilise power during WiFi activity, as is often required or clone boards.

## How it Works

The first key function is the `WiFiCheck()` function, which ensures WiFi is connected whenever called. The function is called before any ThingSpeak calls.
```cpp
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
```

The other key function is the `update()` function, which is in charge of reading from the sensor, and sending the updated values to the ThingSpeak channel.
A `while(...){...}` loop is used to reread the sensor until both a valid temperature and a valid humidity are read.
A `do{...}while(...);` loop is used to attempt to push the updated readings to ThingSpeak, or print the status code if an error occurs (see status code table below).
The readings are cast to integers to better show changes on the ThingSpeak graphs, however the float value can also be used.
```cpp
void update(){
  float temp = dht.readTemperature();
  float rH = dht.readHumidity();

  while(isnan(rH) || isnan(temp)){
    delay(1000);
    temp = dht.readTemperature();
    rH = dht.readHumidity();
  }

  int status;
  do{
    WiFiCheck();
    ThingSpeak.setField(1, (int)rH);
    ThingSpeak.setField(2, (int)temp);
    status = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(status != 200){
      Serial.print("ThingSpeak error: ");
      Serial.println(status);
      delay(1000);
    }
  }while(status != 200);
}
```

The main code loop simply calls `update()` every interval (set by the `updateTimeout` value). It works by checking if the interval has passed, in which case it will increase `lastUpdate` by the time interval and then calls the `update()` function.
`lastUpdate` is increased by `updateTimeout` to try to ensure the correct number of updates even if an error has to be handled.
```cpp
  if(millis() - lastUpdate >= updateTimeout){
    lastUpdate += updateTimeout;
    update();
  }
```

## Usage
Requires the following libraries:
* ThingSpeak
* DHT
* ESP8266 WiFi

Simply connect the DHT11 sensor to D1 of the ESP8266 D1 Mini clone and update the details in `secrets.h`.

## ThingSpeak Status Codes
| Value | Meaning |
|------|-|
|  200 | OK / Success |
|  404 | Incorrect API key (or invalid ThingSpeak server address) |
| -101 | Value is out of range or string is too long (> 255 characters) |
| -201 | Invalid field number specified |
| -210 | setField() was not called before writeFields() |
| -301 | Failed to connect to ThingSpeak |
| -302 | Unexpected failure during write to ThingSpeak |
| -303 | Unable to parse response |
| -304 | Timeout waiting for server to respond |
| -401 | Point was not inserted (most probable cause is the rate limit of once every 15 seconds) |
|    0 | Other error |

(From <https://github.com/mathworks/thingspeak-arduino>)