#pragma once

#include "Area.h"

namespace mms {

class MetersSquared : public Area {

public:
    MetersSquared();
    MetersSquared(double meters);
    MetersSquared(const Area& area);
    MetersSquared operator+(const Area& area) const;
    MetersSquared operator-(const Area& area) const;
    MetersSquared operator*(double factor) const;
    double operator/(const Area& area) const;

};

} // namespace mms
