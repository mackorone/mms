#include "MazeFileUtilities.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

namespace sim{

// Initially, path is a path to the binary file that is executed
std::string getMazeFileDirPath(std::string path){
    
    // Ensure that the path begins with a relative directory location
    if (path.at(0) != '.' && path.at(0) != '/'){
        path.insert(0, "./");
    }
    
    // Gets the path to the "mms" dir based on where the bin file was executed
    for (int i = 0; i < 2; i += 1){

        int index = path.find_last_of("/\\");
        path = path.substr(0, index);

        // Ensures that the maze can be loaded from all directories (end cases)
        if (i == 0 && (path == ".." || path == ".")){
            // If the bin path is to the current or parent directory, then the
            // mms path must be the parent directory of directory to which we
            // already have a path
            path += "/..";
            break;
        }
    }

    // Append mazeFile directory path from the root of the project
    path += "/src/mazeFiles/";

    return path;
}

} // namespace sim
