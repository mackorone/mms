#pragma once

#include <string>

namespace sim {

class Directory {

public:

    // The Directory class is not constructible
    Directory() = delete;

    // mms/res
    static std::string getResDirectory();

    // mms/res/fonts
    static std::string getResFontsDirectory();

    // mms/res/maze
    static std::string getResMazeDirectory();

    // mms/res/mouse
    static std::string getResMouseDirectory();

    // mms/res/shaders
    static std::string getResShadersDirectory();

    // mms/res/imgs
    static std::string getResImgsDirectory();

    // mms/run
    static std::string getRunDirectory();

private:

    // mms
    static std::string getProjectDirectory();

};

} // namespace sim
