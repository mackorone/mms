#pragma once

namespace sim {

class Angle {

public:
    virtual ~Angle() = 0;
    float getRadians() const;
    float getDegrees() const;

protected:
    Angle();
    float m_radians;

};

} // namespace sim
