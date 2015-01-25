#pragma once

namespace sim {

class Point {

public:
    Point();
    Point(float x, float y);
    Point(const Point& point);
    float getX() const;
    float getY() const;
    Point translate(float dx, float dy) const;

private:
    float m_x;
    float m_y;
};

} // namespace sim
