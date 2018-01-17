#include "Area.h"

namespace mms {

Area::Area() : Area(0.0) {
}

Area Area::MetersSquared(double metersSquared) {
    return Area(metersSquared);
}

double Area::getMetersSquared() const {
    return m_metersSquared;
}

Area Area::operator*(double factor) const {
    return Area(m_metersSquared * factor);
}

Area Area::operator+(const Area& other) const {
    return Area(m_metersSquared + other.m_metersSquared);
}

Area Area::operator-(const Area& other) const {
    return Area(m_metersSquared - other.m_metersSquared);
}

Area::Area(double metersSquared) : m_metersSquared(metersSquared) {
}

} // namespace mms
