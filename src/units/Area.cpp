#include "Area.h"

namespace sim {

Area::Area() : m_metersSquared(0.0) {
}

Area::~Area() {
}

float Area::getMetersSquared() const {
    return m_metersSquared;
}

} // namespace sim
