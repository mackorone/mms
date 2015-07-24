#pragma once

namespace sim {

class Angle {

public:
    virtual ~Angle() = 0;
    double getRadians() const;
    double getDegrees() const;
    bool operator<(const Angle& angle);

protected:
    Angle();
    double m_radians;

};

} // namespace sim
