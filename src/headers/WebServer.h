#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <LittleFS.h>
#include <WiFiUdp.h>
#include "headers/Sensor.h"

class WebServer {
    public:
    WebServer(uint16_t port, Sensor& sensor);
    void begin();
    void handleClient();
    void saveData();

    private:
    void handleRoot();
    void handleData();

    ESP8266WebServer server;
    Sensor& sensor;
    WiFiUDP ntpUDP;
    NTPClient timeClient;
};

#endif