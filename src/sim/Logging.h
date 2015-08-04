#pragma once

#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_THREAD_SAFE
#include <easylogging++.h>

#define MAZE_STRING "maze"
#define MOUSE_STRING "mouse"
#define SIM_STRING "sim"

// TODO: Logging by class, not by section...
#define LOG_MAZE(LEVEL) CLOG(LEVEL, MAZE_STRING)
#define LOG_MOUSE(LEVEL) CLOG(LEVEL, MOUSE_STRING)
#define LOG_SIM(LEVEL) CLOG(LEVEL, SIM_STRING)

namespace sim {

class Logging {

public:

    static void initialize();

private:

    // A private constructor is used to ensure only one instance of this class exists
    Logging();
};

} // namespace sim
