#include "Point.h"

namespace sim {

Point::Point() : m_x(0.0), m_y(0.0)
{ }

Point::Point(float x, float y) : m_x(x), m_y(y)
{ }

Point::Point(const Point& point) : m_x(point.getX()), m_y(point.getY())
{ }

float Point::getX() const {
    return m_x;
}

float Point::getY() const {
    return m_y;
}

Point Point::translate(float dx, float dy) const {
    return Point(m_x + dx, m_y + dy);
}

} // namespace sim
