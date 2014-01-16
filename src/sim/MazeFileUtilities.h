#ifndef SIM_MAZEFILEUTILITIES_H_
#define SIM_MAZEFILEUTILITIES_H_

#include <string>

namespace sim{

std::string getMazeFilePath(std::string path, std::string mazeFile);
int mazeFileWidth(std::string mazeFilePath);
int mazeFileHeight(std::string mazeFilePath);
bool checkValidMazeFile(std::string mazeFilePath);

} // namespace sim
#endif
