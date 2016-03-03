#include "Directory.h"

#ifdef _WIN32
    #include "Windows.h"
#else
    #include <limits.h>
    #include <unistd.h>
#endif

namespace sim {

std::string Directory::getResDirectory() {
    return getProjectDirectory() + "res/";
}

std::string Directory::getResFontsDirectory() {
    return getResDirectory() + "fonts/";
}

std::string Directory::getResMazeDirectory() {
    return getResDirectory() + "maze/";
}

std::string Directory::getResMouseDirectory() {
    return getResDirectory() + "mouse/";
}

std::string Directory::getResShadersDirectory() {
// XXX: Temporary workaround for OSX
#ifdef __APPLE__
    return getResDirectory() + "shaders/OSX/";
#else
    return getResDirectory() + "shaders/";
#endif
}

std::string Directory::getResImgsDirectory() {
    return getResDirectory() + "imgs/";
}

std::string Directory::getRunDirectory() {
    return getProjectDirectory() + "run/";
}

std::string Directory::getProjectDirectory() {

    // This approach is claimed to be more reliable than argv[0] on windows
    // and linux.  On Windows GetModuleFileName is the directory to the executable
    // which is located in /mms/src/Debug/.  On linux /proc/self/exe is a path to exe.
    // This approach does not work for all flavors to Linux, but should work on most
    // common ones. The executable on Linux is located in "mms/bin".

    std::string path;

#ifdef __linux
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    path = std::string(result, (count > 0) ? count : 0);
    path = path.substr(0, path.find_last_of("\\/")); // Remove the executable name as it is part of path
    path = path.substr(0, path.find_last_of("\\/")) + "/"; // Remove the "bin" from the path
#elif _WIN32
    char result[MAX_PATH];
    path = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
    path = path.substr(0, path.find_last_of("\\/"));; // Remove the executable name as it is part of path
    path += "\\..\\..\\"; // Point to /mms/
    // Windows uses \ in directory
#elif __APPLE__
    // TODO: upforgrabs
    // Make this not hard-coded, implement an actual __APPLE__ version of this function
    path = "/Users/samsiegart/mms/";
#endif

    return path;
}

} // namespace sim
