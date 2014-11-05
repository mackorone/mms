#include "MazeFileUtilities.h"

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

namespace sim {

std::string getProjectDirectory(){

    // This approach is claimed to be more reliable than argv[0] on windows
    // and linux.  On Windows GetModuleFileName is the directory to the executable
    // which is located in /mms/src/Debug/.  On linux /proc/self/exe is a path to exe.
    // This aproach does not work for all flavors to Linux, should on the common.
    // executable on Linux is located at /mms/bin/

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
    path += "/../"; // Point to /mms/
#endif

    return path;
}

std::string getMazeFileDirPath(){
    
    std::string path;

    path = getProjectDirectory();

    // Append mazeFile directory path from the root of the project
    path += "src/maze_files/";

    return path;
}

/*int mazeFileWidth(std::string mazeFilePath){

}

int mazeFileHeight(std::string mazeFilePath){

}
*/

bool checkValidMazeFile(std::string mazeFilePath){

    // TODO: Ensure that you have unique y values for all x values
    // TODO: Ensure that the walls line up
    // TODO: Make a mazeFileValidator - gets height, width, valid, etc

    // This function validates following:
    // 1.) There are an equal number of y values for each x value
    // 2.) All the x and y values are unique
    // 3.) The walls are valid

    // Create the file object
    std::ifstream file(mazeFilePath.c_str());

    // Initialize a string variable
    std::string line("");

    if (file.is_open()){

        // Vector that counts the number of y values colums for each x value // TODO
        std::vector<int> counts;
        counts.push_back(0);
        int xValue = 0;

        while (getline(file, line)){
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                 std::back_inserter<std::vector<std::string> >(tokens));

            // Counting logic // TODO
            /*if (tokens.at(0) == xValue){
                counts.at(xValue) += 1;
            }
            else{
                xValue = tokens.at(0);
                counts.push_back(1); 
            }*/
        }
    
        file.close();

        /*int numberOfYValues = counts.at(0);
        for (int i = 1; i < counts.size(); i += 1){
            if (counts.at(i) != numberOfYValues){
                return 0;
            }
        }*/

    }
    
    // false indicates invalid file
    return false;
}

bool checkValidMazeFileTom(std::string mazeFilePath, int height, int width){

    // Create the file object
    std::ifstream file(mazeFilePath.c_str());

    if (!file.is_open())
        return false; // Error opening file

    // Initialize a string variable
    std::string line("");

    int x_pos = 0;
    int y_pos = 0;

    int expected_x_pos = 0;
    int expected_y_pos = 0;

    int current_int = 0;

    while (std::getline(file, line)){
        for (int i = 0; i < 6; i += 1){

            try { // When did this happen << ?
                current_int = std::stoi(line, nullptr, 10);  //Attempt to get an integer which is followed by a space
            }
            catch (const std::invalid_argument& ia) {
                return false; // If not then something in the maze file is not valid
            }
            if (i == 0){ // X Pos
                if (current_int != expected_x_pos){
                    return false;
                }
                x_pos = current_int;
                if (x_pos == width)
                    return false; // Too many lines, RUN
            }
            if (i == 1){ // Y Pos
                if (current_int != expected_y_pos){
                    return false;
                }
                if (current_int == height - 1) { // Wrap Around
                    expected_y_pos = 0;
                    expected_x_pos += 1; // X-Pos will increase
                } else{
                    expected_y_pos += 1;
                }
                x_pos = current_int;
            }
            if (i > 1){ // Wall Values - check if 1 or 0
                if (current_int != 0 && current_int != 1){
                    return false;
                }
            } 
            std::size_t found = line.find_first_of(" ");
            if (found != std::string::npos) { // extra space exists in the string
                line = line.substr(line.find_first_of(" ") + 1, line.length());
                if (i == 5){
                    return false; // return false if more spaces after last digit remain
                }
            }
            else{
                if (i != 5)
                    return false; // If no more spaces before the last digit that means the line
                                  // does not have enough digits
            }
        }
        if (expected_x_pos == width)
            break;
    }
  
    if (x_pos != width && y_pos != 0)
        return false; //Not all lines present in the last group

    return true; //should be fine

}

} // namespace sim
