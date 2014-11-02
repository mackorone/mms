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
    path += "src/mazeFiles/";

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

} // namespace sim
