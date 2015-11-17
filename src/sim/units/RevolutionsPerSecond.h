#pragma once

#include "AngularVelocity.h"

namespace sim {

class RevolutionsPerSecond : public AngularVelocity {

public:
    RevolutionsPerSecond(double revolutionsPerSecond);
    RevolutionsPerSecond(const AngularVelocity& angularVelocity);

};

} // namespace sim
