#include "Utilities.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#ifdef _WIN32
    #include <windows.h>
#elif __linux
    #include <limits.h>
    #include <unistd.h>
#endif

namespace sim {

void sleep(const Time& time) {
    ASSERT(time.getMilliseconds() > 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(time.getMilliseconds())));
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

bool isBool(std::string str) {
    return 0 == str.compare("true") || 0 == str.compare("false");
}

bool isInt(std::string str) {
    
    // If there's a negative sign, remove it
    if (!str.empty() && '-' == str.at(0)) {
        str = str.substr(1, str.size() - 1);
    }

    // Ensure it's not an empty string
    if (str.empty()) {
        return false;
    }

    // Ensure there are no leading zeros
    if ('0' == str.at(0)) {
        return 1 == str.size();
    }

    // Ensure that we have all digits
    for (int i = 0; i < str.size(); i += 1) {
        if (!('0' <= str.at(i) && str.at(i) <= '9')) {
            return false;
        }
    }

    return true;
}

bool isFloat(std::string str) {
    // TODO
    return false;
}

bool strToBool(std::string str) {
    ASSERT(isBool(str));
    return 0 == str.compare("true");
}

int strToInt(std::string str) {
    ASSERT(isInt(str));
    return atoi(str.c_str()); // TODO
}

float strToFloat(std::string str) {
    ASSERT(isFloat(str));
    return stof(str); // TODO
}


std::vector<std::string> tokenize(std::string str) {

    std::vector<std::string> tokens;
    std::string word = ""; 

    for (int i = 0; i < str.size(); ++i) {
        if (str.at(i) == ' ') {
            if (!word.empty()) {
                tokens.push_back(word);
                word = ""; 
            }   
        }   
        else {
            word += str.at(i);
        }   
    }   
    if (!word.empty()) {
        tokens.push_back(word);
        word = ""; 
    }   

    return tokens;
}

// TODO: delete this
bool checkValidMazeFileTom(std::string mazeFilePath, int height, int width) {

    // Create the file object
    std::ifstream file(mazeFilePath.c_str());

    // Error opening file
    if (!file.is_open()) {
        // std::cout
        return false;
    }

    // Initialize a string variable
    std::string line("");

    int expected_x_pos = 0;
    int expected_y_pos = -1;

    int current_int = 0;

    while (std::getline(file, line)) {

        if (!hasOnlyDigits(line)) { // The line has some non digit characters
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

            if (line.at(0) == ' ') { //Check that the first character is not a space
                                    //This would only occur if two spaces apear in a row
                return false;
            }

            current_int = std::strtol(line.c_str(), nullptr, 10);  //Get the next int an integer which is followed by a space
            
            if (i == 0 && current_int != expected_x_pos) { // X Pos on line does not match the expected X Position
                return false;
            }
            if (i == 1) { // Y Pos Handliing
                if (current_int != expected_y_pos) { // Y Pos on line does not match the expected Y Position
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

    return true; //should be fine
}

bool hasOnlyDigits(const std::string &str) { // TODO: delete this
    return str.find_first_not_of("0123456789 ") == std::string::npos;
}

// -------------------------- Graphics Utilities ------------------------------
 
// Supposedly this should work...
Point physicalToOpenGl(const Point& point) {
    return Point(convertHorizontalPoint(rtp(point.getX())), convertVerticalPoint(rtp(point.getY())));
}
 
// TODO: Implementation independent of params WINDOW_WIDTH
float convertHorizontalPoint(float coordinate) {
    return ((coordinate / P()->windowWidth()) - 0.5f) * 2;
}
 
// TODO: Implementation independent of params WINDOW_HEIGHT
float convertVerticalPoint(float coordinate) {
    return ((coordinate / P()->windowHeight()) - 0.5f) * 2;
}

float rtp(float meters) {
    return P()->pixelsPerMeter() * meters;
}

struct mazeTile {

    int x_pos;
    int y_pos;
    int wallNorth;
    int wallEast;
    int wallSouth;
    int wallWest;

};

// TODO
bool mackValid(std::string mazeFilePath) {

    // TODO: This should ensure a PHYSICALLY POSSIBLE MAZE
    // Format requires that:
    // - File exists
    // - The correct number of rows and columns
    // - rows and cols are in expected order
    // - The rows and columns labeled correctly
    // - Walls are all valid
    // - The maze is surrounded by walls
    // This means that the maze can be unsolvable and any size and shape

    // TODO: extract the width and height from the file

    // TODO: Do we want to perform maze validation on autogenerated mazes? probably, just for safe keeping and consistency

    // TODO: Function for specific rules
    // TODO: Maze validation - has a path to the center, no wall following robot, etc.

    // Create the file object
    // TODO: Take this out of the maze
    std::ifstream file(mazeFilePath.c_str());

    // Error opening file
    if (!file.is_open()) {
        // TODO: Is this really the best error message here...
        std::cout << "Invalid maze file: could not open \"" << mazeFilePath << "\"" << std::endl;
        return false;
    }

    // Initialize a string variable
    std::string line("");

    // Current x and y position
    int x = 0;
    int y = 0;

    // Overall width and height of the maze // TODO: Needed??
    int width = -1;
    int height = -1;

    std::vector<std::vector<mazeTile>> maze; // Store the values of the maze

    while (std::getline(file, line)) {

        // Extract the whitespace separated tokens
        std::vector<std::string> tokens = tokenize(line);

        // Check to see that there are exactly six entries...
        if (6 != tokens.size()) {
            // TODO: Print error strings
            return false;
        }

        // ... all of which are numeric
        for (int i = 0; i < tokens.size(); i += 1) {
            if (!isInt(tokens.at(i))) {
                // TODO: Print error strings
                return false;
            }
        }

        mazeTile tile;

        tile.x_pos = atoi(tokens.at(0).c_str()); // X position
        tile.y_pos = atoi(tokens.at(1).c_str()); // Y position
        tile.wallNorth = (1 == atoi(tokens.at(2).c_str())); // 1 if north wall, else 0
        tile.wallEast = (1 == atoi(tokens.at(3).c_str())); // 1 if  east wall, else 0
        tile.wallSouth = (1 == atoi(tokens.at(4).c_str())); // 1 if south wall, else 0
        tile.wallWest = (1 == atoi(tokens.at(5).c_str())); // 1 if  west wall, else 0

        // TODO: First, each line should have precisely six things
    }
    return true; //should be fine
}

std::pair<int, int> getMazeWidthAndHeight() {

}

} // namespace sim
