#pragma once

#include "Area.h"

namespace sim {

class MetersSquared : public Area {

public:
    MetersSquared();
    MetersSquared(float meters);
    MetersSquared(const Area& area);
    MetersSquared operator+(const Area& area) const;
    MetersSquared operator-(const Area& area) const;
    MetersSquared operator*(float factor) const;

};

} // namespace sim
