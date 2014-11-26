#include "Utilities.h"

#include <chrono>
#include <thread>

// TODO: Are all of these needed???
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>


#ifdef _WIN32
    #include <windows.h>
#elif __linux
    #include <limits.h>
    #include <unistd.h>
#endif

#include "Param.h"


namespace sim {

void sleep(int ms) {
	 std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

std::string getProjectDirectory() {

    // This approach is claimed to be more reliable than argv[0] on windows
    // and linux.  On Windows GetModuleFileName is the directory to the executable
    // which is located in /mms/src/Debug/.  On linux /proc/self/exe is a path to exe.
    // This approach does not work for all flavors to Linux, should on the common.
    // The executable on Linux is located in "mms/bin".

    std::string path;

#ifdef _WIN32
    char result[MAX_PATH];
    path = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
    path = path.substr(0, path.find_last_of("\\/"));; // Remove the executable name as it is part of path
    path += "\\..\\..\\"; // Point to /mms/
    // Windows uses \ in directory
#elif __linux
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    path = std::string(result, (count > 0) ? count : 0);
    path = path.substr(0, path.find_last_of("\\/"));; // Remove the executable name as it is part of path
    path += "/../";
#endif

    return path;
}

bool checkValidMazeFileTom(std::string mazeFilePath, int height, int width){

    // TODO:
    // The correct number of rows and columns
    // The rows and columns labeled correctly
    // Walls are all valid
    // The maze is surrounded by walls

    // Create the file object
    std::ifstream file(mazeFilePath.c_str());

    // Error opening file
    if (!file.is_open()) {
        return false;
    }

    // Initialize a string variable
    std::string line("");

    int expected_x_pos = 0;
    int expected_y_pos = -1;

    int current_int = 0;

    while (std::getline(file, line)) {

        if (!hasOnlyDigits(line)){ // The line has some non digit characters
            return false;
        }
        
        expected_y_pos += 1; // Add one to the expected Y_position

        if (expected_y_pos == height) { // Wrap Around
            expected_y_pos = 0;
            expected_x_pos += 1; // X-Pos will increase
        }

        if (expected_x_pos == width) {
            return false; // Too many lines, RUN
        }

        for (int i = 0; i < 6; i += 1) {

            if (line.at(0) == ' '){ //Check that the first character is not a space
                                    //This would only occur if two spaces apear in a row
                return false;
            }

            current_int = std::strtol(line.c_str(), nullptr, 10);  //Get the next int an integer which is followed by a space
            
            if (i == 0 && current_int != expected_x_pos){ // X Pos on line does not match the expected X Position
                return false;
            }
            if (i == 1){ // Y Pos Handliing
                if (current_int != expected_y_pos){ // Y Pos on line does not match the expected Y Position
                    return false;
                }
            }

            if (i > 1 && !(current_int == 0 || current_int == 1)) { // Wall Values - check if 1 or 0
                return false;
            } 

            std::size_t found = line.find_first_of(" ");
            if (found != std::string::npos) { // extra space exists in the string
                line = line.substr(line.find_first_of(" ") + 1, line.length());
                if (i == 5) {
                    return false; // return false if more spaces after last digit remain
                }
            }
            else if (i != 5) {
                return false; // If no more spaces before the last digit that means the line
                                  // does not have enough digits
            }
        }
    }
  
    if (expected_x_pos != width - 1 && expected_y_pos != 0) {
        return false; // Not all lines present in the last group
    }

    // TODO: Maze validation - has a path to the center, no wall following robot, etc.

    // TODO: Make sure the neighboring tiles have corresponding walls (if a tile has
    // a wall to the right, the tile to its right should have a wall to the left, etc.)

    return true; //should be fine
}

bool hasOnlyDigits(const std::string &str) {
    return str.find_first_not_of("0123456789 ") == std::string::npos;
}

// -------------------------- Graphics Utilities ------------------------------
 
// TODO: Implementation independent of params WINDOW_WIDTH
float convertHorizontalPoint(float coordinate) {
    return ((coordinate / P()->WINDOW_WIDTH()) - 0.5f) * 2;
}
 
// TODO: Implementation independent of params WINDOW_HEIGHT
float convertVerticalPoint(float coordinate) {
    return ((coordinate / P()->WINDOW_HEIGHT()) - 0.5f) * 2;
}


} // namespace sim
