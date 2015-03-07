#pragma once

namespace sim {

class Area {

public:
    virtual ~Area() = 0;
    float getMetersSquared() const;

protected:
    Area();
    float m_metersSquared;

};

} // namespace sim
