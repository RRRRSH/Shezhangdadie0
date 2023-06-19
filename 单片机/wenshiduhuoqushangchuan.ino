#include "ESP8266.h"
#include "dht11.h"
#include "SoftwareSerial.h"

#define WIFI_SSID ""    // WIFI
#define WIFI_PASSWORD "" // WIFI password
#define API_HOST_NAME "api.heclouds.com" // API host name
#define DEVICE_ID "1099589303" // OneNet device ID
#define API_HOST_PORT (80) // API port
String APIKey = "gRIkH6CDKO9XrN4ShCPqGseRYHI="; // API key associated with the device

#define SENSOR_INTERVAL 5000 // Define sensor sampling and sending interval

dht11 DHT11;

// Define the pins for DHT11 sensor connection to Arduino
#define DHT11_PIN 3

SoftwareSerial mySerial(4, 2);
ESP8266 wifi(mySerial);

unsigned long networkTime = millis(); // Data upload time to server

void setup() {
  mySerial.begin(115200); // Initialize software serial
  Serial.begin(9600); // Initialize serial communication
  Serial.print("Setup begin\r\n");
  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());
  
  if (wifi.setOprToStation()) {
    Serial.print("To station OK\r\n");
  } else {
    Serial.print("To station error\r\n");
  }
  
  if (wifi.joinAP(WIFI_SSID, WIFI_PASSWORD)) { // Connect to WiFi
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }
  
  Serial.println("");
  Serial.print("DHT11 library version: ");
  Serial.println(DHT11LIB_VERSION);
  
  mySerial.println("AT+UART_CUR=9600,8,1,0,0");
  mySerial.begin(9600);
  Serial.println("Setup end\r\n");
}

void loop() {
  if (networkTime > millis())
    networkTime = millis();

  if (millis() - networkTime > SENSOR_INTERVAL) { // Sending data interval
    int checkResult = DHT11.read(DHT11_PIN);

    Serial.print("Read sensor: ");
    switch (checkResult) {
      case DHTLIB_OK:
        Serial.println("OK");
        break;
      case DHTLIB_ERROR_CHECKSUM:
        Serial.println("Checksum error");
        break;
      case DHTLIB_ERROR_TIMEOUT:
        Serial.println("Timeout error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }

    float sensorHumidity = (float)DHT11.humidity;
    float sensorTemperature = (float)DHT11.temperature;
    Serial.print("Humidity (%): ");
    Serial.println(sensorHumidity, 2);

    Serial.print("Temperature (oC): ");
    Serial.println(sensorTemperature, 2);
    Serial.println("");

    if (wifi.createTCP(API_HOST_NAME, API_HOST_PORT)) { // Establish TCP connection
      Serial.print("Create TCP OK\r\n");
      char buffer[10];
      // Concatenate the 'data' field string to send
      String jsonToSend = "{\"Temperature\":";
      dtostrf(sensorTemperature, 1, 2, buffer);
      jsonToSend += "\"" + String(buffer) + "\"";
      jsonToSend += ",\"Humidity\":";
      dtostrf(sensorHumidity, 1, 2, buffer);
      jsonToSend += "\"" + String(buffer) + "\"";
      jsonToSend += "}";

      String postString = "POST /devices/";
      postString += DEVICE_ID;
      postString += "/datapoints?type=3 HTTP/1.1";
      postString += "\r\n";
      postString += "api-key:";
      postString += APIKey;
      postString += "\r\n";
      postString += "Host:api.heclouds.com\r\n";
      postString += "Connection:close\r\n";
      postString += "Content-Length:";
      postString += jsonToSend.length();
      postString += "\r\n";
      postString += "\r\n";
      postString += jsonToSend;
      postString += "\r\n";
      postString += "\r\n";
      postString += "\r\n";

      const char *postDataArray = postString.c_str(); // Convert 'postString' to a char array

      Serial.println(postDataArray);
      wifi.send((const uint8_t *)postDataArray, strlen(postDataArray)); // Send the command
      Serial.println("Send success");

      if (wifi.releaseTCP()) { // Release TCP connection
        Serial.print("Release TCP OK\r\n");
      } else {
        Serial.print("Release TCP error\r\n");
      }

      postDataArray = NULL; // Clear the array, wait for the next data transmission
    } else {
      Serial.print("Create TCP error\r\n");
    }

    Serial.println("");

    networkTime = millis();
  }
}

