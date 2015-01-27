#pragma once

namespace sim {

class Speed {

public:
    virtual ~Speed() = 0;
    float getMetersPerSecond() const;

protected:
    Speed();
    float m_metersPerSecond;

};

} // namespace sim
