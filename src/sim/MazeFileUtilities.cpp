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
