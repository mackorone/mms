#pragma once

#include "Sensor.h"

namespace sim {

class SensorInterface {

public:
    SensorInterface(Sensor* sensor);
    float read();

private:
    Sensor* m_sensor;

};

} // namespace sim
