#include "Triangle.h"

namespace sim {

Triangle::Triangle() {
}

Triangle::Triangle(const Cartesian& p1, const Cartesian& p2, const Cartesian& p3) {
    m_p1 = p1;
    m_p2 = p2;
    m_p3 = p3;
}

Cartesian Triangle::getP1() const {
    return m_p1;
}

Cartesian Triangle::getP2() const {
    return m_p2;
}

Cartesian Triangle::getP3() const {
    return m_p3;
}

} // namespace sim
