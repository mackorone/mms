#include "Area.h"

namespace mms {

Area::Area() : m_metersSquared(0.0) {
}

Area::~Area() {
}

double Area::getMetersSquared() const {
    return m_metersSquared;
}

} // namespace mms
