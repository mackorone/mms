#pragma once

#include "AngularVelocity.h"

namespace mms {

class RevolutionsPerMinute : public AngularVelocity {

public:
    RevolutionsPerMinute(double revolutionsPerMinute);
    RevolutionsPerMinute(const AngularVelocity& angularVelocity);

};

} // namespace mms
