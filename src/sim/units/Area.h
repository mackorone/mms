#pragma once

namespace mms {

class Area {

public:
    virtual ~Area() = 0;
    double getMetersSquared() const;

protected:
    Area();
    double m_metersSquared;

};

} // namespace mms
