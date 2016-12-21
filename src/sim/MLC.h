#pragma once

#include "Controller.h"
#include "Lens.h"
#include "Mouse.h"

namespace mms {

struct MLC {
    
    // The model of the mouse
    Mouse* mouse;

    // The maze, as perceived by the mouse
    Lens* lens;

    // The interface between the algo and the mouse
    Controller* controller;
};

} // namespace mms
