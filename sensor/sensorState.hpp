#ifndef SENSORSTATE_HPP_
#define SENSORSTATE_HPP_

#include<cstdint>

class SensorState {
public:
    const static uint8_t DEAD;
    const static uint8_t WORK;
    const static uint8_t FULL;
};

#endif