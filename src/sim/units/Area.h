#pragma once

namespace sim {

class Area {

public:
    virtual ~Area() = 0;
    double getMetersSquared() const;

protected:
    Area();
    double m_metersSquared;

};

} // namespace sim
