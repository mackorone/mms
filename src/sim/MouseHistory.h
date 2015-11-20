#pragma once

#include <list>

#include "units/Distance.h"
#include "units/Duration.h"
#include "units/Angle.h"

namespace sim {

// TODO: Perhaps like a separate playback or something like that... ghosting???

class MouseHistory {

public:
    MouseHistory();
    void append(const Distance& x, const Distance& y, const Angle& rotation, const Duration& sinceStart);
    // TODO: Figure out how to draw the path behind

private:

    typedef struct {
        double x; // x-position, in meters
        double y; // y-position, in meters
        double r; // rotation, in radians
        double t; // time, in seconds since the start of the run
    } position;

    std::list<position> m_history;

};

} // namespace sim
