#pragma once

namespace sim {

class Coordinate {

public:
    virtual ~Coordinate() = 0;
    float getX() const;
    float getY() const;
    float getRho() const;
    float getTheta() const;

protected:
    Coordinate();
    float m_x;
    float m_y;

};

} // namespace sim
