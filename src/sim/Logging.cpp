#include "Logging.h"

#include "SimUtilities.h"

INITIALIZE_EASYLOGGINGPP

namespace sim {

void Logging::initialize() {

    // TODO: MACK - Define ideal logging behavior, then let someone else implement it
    // TODO: Make sure we only initialize here once...
    // TODO: Only keep the N most recent files...
    // TODO: Set truncation size..
    // TODO: New folder for each invokation of the program...
    // TODO: Keep in mind that we might want lots of runs in a unit-test like situation
    // TODO: I mean, we'll probably have run numbers...

    // Note: We assume that this will never fail. If the file doesn't exist
    // already, we'll create it. If it does exists, we'll append to it.

    // Register and configure each of the loggers
    for (const std::string& str : {MAZE_STRING, MOUSE_STRING, SIM_STRING}) {
        el::Loggers::getLogger(str);
        el::Configurations config;
        config.setGlobally(el::ConfigurationType::Filename,
            SimUtilities::getProjectDirectory() + "log/" + SimUtilities::getDateTime() + "/logs.txt");
        el::Loggers::reconfigureLogger(str, config);
    }

    LOG_SIM(INFO) << "Mack Test";
}

} // namespace sim
