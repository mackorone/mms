#pragma once

namespace sim {

class Distance {

public:
    virtual ~Distance() = 0;
    float getMeters() const;
    float getCentimeters() const;

protected:
    Distance();
    float m_meters;

};

} // namespace sim
