#pragma once

namespace sim {

class Angle {

public:
    virtual ~Angle() = 0;
    float getRadians() const;
    float getDegrees() const;
    bool operator<(const Angle& angle);

protected:
    Angle();
    float m_radians;

};

} // namespace sim
