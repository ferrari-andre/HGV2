#include <ESP8266WiFi.h>
#include "headers/Sensor.h"
#include "headers/WebServer.h"
#include <Ticker.h>

const char* ssid = "";
const char* password = "";

Sensor sensor(D7, DHT22);
WebServer webServer(80, sensor);
Ticker dataTicker;


void saveData() {
  webServer.saveData();
}

void setup() {
  Serial.begin(9600);
  sensor.start();

  WiFi.begin(ssid, password);
  Serial.println("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.println("Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  webServer.begin();
  dataTicker.attach(3600, saveData);
}


void loop() {
  webServer.handleClient();
}