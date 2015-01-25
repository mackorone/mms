#pragma once

namespace sim {

class Distance {

public:
    virtual ~Distance();
    float getMeters() const;
    float getCentimeters() const;

protected:
    Distance(); // Abstract class
    float m_meters;

};

} // namespace sim
