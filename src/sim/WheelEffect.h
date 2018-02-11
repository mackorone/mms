#pragma once

#include "units/AngularVelocity.h"
#include "units/Speed.h"

namespace mms {

struct WheelEffect {
    Speed forwardEffect;
    Speed sidewaysEffect;
    AngularVelocity turnEffect;
};

} // namespace mms
