#pragma once

#include <string>

namespace sim {

// The function returns the directory of the maze file.
// It takes in the name of the executed binary file for location reference.
std::string getMazeFileDirPath(std::string path);

int mazeFileWidth(std::string mazeFilePath);

int mazeFileHeight(std::string mazeFilePath);

bool checkValidMazeFile(std::string mazeFilePath);

} // namespace sim
