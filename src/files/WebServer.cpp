#include "headers/WebServer.h"

WebServer::WebServer(uint16_t port, Sensor& sensor) : server(port), sensor(sensor), timeClient(ntpUDP, "pool.ntp.org", -3 * 3600, 60000) {}

void WebServer::begin() {
    timeClient.begin();
    if (!LittleFS.begin()) {
        Serial.println("Falha ao montar sistema de arquivos.");
        return;
    }

    server.on("/", std::bind(&WebServer::handleRoot, this));
    server.on("/data", std::bind(&WebServer::handleData, this));
    server.begin();
    Serial.println("Server ON.");
}

void WebServer::handleClient() {
    server.handleClient();
}

void WebServer::saveData() {
    timeClient.update();

    float temperatura = sensor.lerTemperatura();
    float umidade = sensor.lerUmidade();

    if (isnan(temperatura) || isnan(umidade)) {
        Serial.println("Falha ao ler DHT22");
        return;
    }

    String dataFormatada = timeClient.getFormattedTime();
    char json[200];
    snprintf(json, sizeof(json), "{\"temperatura\":%0.2f,\"umidade\":%0.2f,\"timestamp\":\"%s\"}", temperatura, umidade, dataFormatada.c_str());

    File file = LittleFS.open("/data.txt", "a");
    if (!file) {
        Serial.println("Falha ao abrir arquivo.");
        return;
    }

    file.println(json);
    file.close();
}

void WebServer::handleRoot() {
    timeClient.update();
    
    float temperatura = sensor.lerTemperatura();
    float umidade = sensor.lerUmidade();

    if (isnan(temperatura) || isnan(umidade)) {
        Serial.println("Falha ao ler DHT22.");
        server.send(500, "application/json", "{\"error\":\"Falha ao ler DHT22.\"}");
        return;
    }

    String dataFormatada = timeClient.getFormattedTime();
    char json[200];
    snprintf(json, sizeof(json), "{\"temperatura\":%0.2f,\"umidade\":%0.2f,\"timestamp\":\"%s\"}", temperatura, umidade, dataFormatada.c_str());
    
    File file = LittleFS.open("/data.txt", "a");
    if (!file) {
        Serial.println("Falha ao abrir arquivo.");
        server.send(500, "application/json", "{\"error\":\"Falha ao abrir arquivo.\"}");
        return;
    }

    file.println(json);
    file.close();

    server.send(200, "application/json", json);
}

void WebServer::handleData() {
    File file = LittleFS.open("/data.txt", "r");
    if (!file) {
        server.send(500, "application/json", "{\"error\":\"Falha ao abrir o arquivo.\"}");
        return;
    }

    String data = file.readString();
    file.close();

    server.send(200, "application/json", data);
}