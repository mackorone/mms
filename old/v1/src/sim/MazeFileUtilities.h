#pragma once

#include <string>

namespace sim {

// The function returns the directory of the maze file.
// It takes in the name of the executed binary file for location reference.
std::string getMazeFileDirPath(std::string path);

} // namespace sim
