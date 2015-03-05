#include "Sensor.h"

namespace sim {

Sensor::Sensor() : m_direction(Meters(0.0), Meters(0.0)), m_range(Meters(0.0)), m_halfWidth(Degrees(5.0)) {
}

} // namespace sim
