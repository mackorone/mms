#include "Coordinate.h"

#include <cmath>

namespace sim {

Coordinate::Coordinate() : m_x(0), m_y(0) {
}

Coordinate::~Coordinate() {
}

float Coordinate::getX() const {
    return m_x;
}

float Coordinate::getY() const {
    return m_y;
}

float Coordinate::getRho() const {
    return sqrt(m_x*m_x + m_y*m_y);
}

float Coordinate::getTheta() const {
    return atan2(m_y, m_x);
}

} // namespace sim
