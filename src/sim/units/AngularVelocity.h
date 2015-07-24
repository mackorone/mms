#pragma once

namespace sim {

class AngularVelocity {

public:
    virtual ~AngularVelocity() = 0;
    double getRadiansPerSecond() const;

protected:
    AngularVelocity();
    double m_radiansPerSecond;

};

} // namespace sim
