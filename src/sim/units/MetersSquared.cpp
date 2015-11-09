#include "MetersSquared.h"

namespace sim {

MetersSquared::MetersSquared() {
    m_metersSquared = 0.0;
}

MetersSquared::MetersSquared(double meters) {
    m_metersSquared = meters;
}

MetersSquared::MetersSquared(const Area& area) {
    m_metersSquared = area.getMetersSquared();
}

MetersSquared MetersSquared::operator+(const Area& area) const {
    return MetersSquared(getMetersSquared() + area.getMetersSquared());
}

MetersSquared MetersSquared::operator-(const Area& area) const {
    return MetersSquared(getMetersSquared() - area.getMetersSquared());
}

MetersSquared MetersSquared::operator*(double factor) const {
    return MetersSquared(getMetersSquared() * factor);
}

double MetersSquared::operator/(const Area& area) const {
    return getMetersSquared() / area.getMetersSquared();
}

} // namespace sim
