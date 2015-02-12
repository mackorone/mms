#pragma once

#include "Wheel.h"

namespace sim {

class WheelInterface {

public:
    WheelInterface(Wheel* wheel);

private:
    Wheel* m_wheel;

};

} // namespace sim
