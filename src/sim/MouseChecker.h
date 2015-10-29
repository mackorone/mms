#pragma once

#include "Mouse.h"

namespace sim {

class MouseChecker {

public:

    // The MouseChecker class is not constructible
    MouseChecker() = delete;

    static bool isDiscreteInterfaceCompatible(const Mouse& mouse);
    static bool isContinuousInterfaceCompatible(const Mouse& mouse);
};

} // namespace sim
