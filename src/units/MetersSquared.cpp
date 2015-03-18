#include "MetersSquared.h"

namespace sim {

MetersSquared::MetersSquared() {
    m_metersSquared = 0.0;
}

MetersSquared::MetersSquared(float meters) {
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

MetersSquared MetersSquared::operator*(float factor) const {
    return MetersSquared(getMetersSquared() * factor);
}

} // namespace sim
