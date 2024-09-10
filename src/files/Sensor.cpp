#include "headers/Sensor.h"

Sensor::Sensor(uint8_t pin, uint8_t type) : dht(pin, type) {}

void Sensor::start() {
    dht.begin();
}

float Sensor::lerTemperatura() {
    return dht.readTemperature();
}

float Sensor::lerUmidade() {
    return dht.readHumidity();
}