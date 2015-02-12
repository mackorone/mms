#include "SensorInterface.h"

namespace sim {

SensorInterface::SensorInterface(Sensor* sensor) : m_sensor(sensor) {
}

float SensorInterface::read() {
    return 0.0;
}

} // namespace sim
