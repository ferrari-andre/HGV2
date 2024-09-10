#ifndef SENSOR_H
#define SENSOR_H

#include <DHT.h>

class Sensor {
    public:
    Sensor(uint8_t pin, uint8_t type);
    void start();
    float lerTemperatura();
    float lerUmidade();

    private:
    DHT dht;
};

#endif