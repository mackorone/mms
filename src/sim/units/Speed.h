#pragma once

namespace sim {

class Speed {

public:
    virtual ~Speed();
    float getMetersPerSecond() const;

protected:
    Speed(); // Abstract class
    float m_metersPerSecond;

};

} // namespace sim
