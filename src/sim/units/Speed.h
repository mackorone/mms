#pragma once

namespace mms {

class Speed {

public:
    virtual ~Speed() = 0;
    double getMetersPerSecond() const;
    bool operator<(const Speed& speed) const;

protected:
    Speed();
    double m_metersPerSecond;

};

} // namespace mms
