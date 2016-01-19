#pragma once

namespace sim {

class Angle {

public:
    virtual ~Angle() = 0;
    double getRadiansZeroTo2pi() const;
    double getDegreesZeroTo360() const;
    double getRadiansNotBounded() const;
    double getDegreesNotBounded() const;
    double getSin() const;
    double getCos() const;
    bool operator<(const Angle& angle) const;

protected:
    Angle();
    double m_radians;

private:
    double getRadians(bool zeroTo2pi) const;
    double getDegrees(bool zeroTo360) const;

};

} // namespace sim
