#pragma once

namespace sim {

class AngularVelocity {

public:
    virtual ~AngularVelocity() = 0;
    float getRadiansPerSecond() const;

protected:
    AngularVelocity();
    float m_radiansPerSecond;

};

} // namespace sim
