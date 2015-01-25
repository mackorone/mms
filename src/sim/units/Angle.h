#pragma once

namespace sim {

class Angle {

public:
    virtual ~Angle();
    float getRadians() const;
    float getDegrees() const;

protected:
    Angle(); // Abstract class
    float m_radians;

};

} // namespace sim
