#pragma once

#include "AngularVelocity.h"

namespace sim {

class RevolutionsPerMinute : public AngularVelocity {

public:
    RevolutionsPerMinute(double revolutionsPerMinute);
    RevolutionsPerMinute(const AngularVelocity& angularVelocity);

};

} // namespace sim
