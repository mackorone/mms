#include "Area.h"

namespace sim {

Area::Area() : m_metersSquared(0.0) {
}

Area::~Area() {
}

double Area::getMetersSquared() const {
    return m_metersSquared;
}

} // namespace sim
