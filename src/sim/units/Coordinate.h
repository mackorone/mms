#pragma once

namespace sim {

class Coordinate {

public:
    virtual ~Coordinate();
    float getX() const;
    float getY() const;
    float getRho() const;
    float getTheta() const;

protected:
    Coordinate(); // Abstract class
    float m_x;
    float m_y;

};

} // namespace sim
