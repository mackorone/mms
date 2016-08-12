#pragma once

#include "AngularVelocity.h"

namespace mms {

class DegreesPerSecond : public AngularVelocity {

public:
    DegreesPerSecond(double degreesPerSecond);
    DegreesPerSecond(const AngularVelocity& angularVelocity);

};

} // namespace mms
