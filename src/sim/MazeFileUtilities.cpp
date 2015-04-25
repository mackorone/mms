#include "MazeFileUtilities.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

#include "Assert.h"
#include "SimUtilities.h"

namespace sim {

bool MazeFileUtilities::isMazeFile(const std::string& mazeFilePath) {

    // TODO: This should ensure the format is correct
    // TODO: SOM
    // Format requires that:
    // - File exists
    // - The correct number of rows and columns
    // - rows and cols are in expected order
    // - The rows and columns labeled correctly

    // First, make sure we've been given a file
    if (!isFile(mazeFilePath)) {
        print("Error: \"" + mazeFilePath + "\" is not a file.");
        return false;
    }

    // Create the file object
    std::ifstream file(mazeFilePath.c_str());

    // Error opening file
    if (!file.is_open()) {
        print("Error: Could not open \"" + mazeFilePath + "\" for maze validation.");
        return false;
    }

    // Initialize a string variable
    std::string line("");

    // Current x and y position
    int x = 0;
    int y = 0;

    // A vector to hold all tiles' values
    std::vector<std::vector<BasicTile>> maze;
    
    //Values for row and column validation
    int rowMax = 0;
    int columnMax = 0;
    int columnCount = 0;
    int prevRow = -1;

    while (std::getline(file, line)) {

        // Extract the whitespace separated tokens
        std::vector<std::string> tokens = tokenize(line);

        // Check to see that there are exactly six entries...
        if (6 != tokens.size()) {
            print("Error: \"" + mazeFilePath + "\" does not contain six entries on each line.");
            return false;
        }

        // ... all of which are numeric
        for (int i = 0; i < tokens.size(); i += 1) {
            if (!isInt(tokens.at(i))) {
                print("Error: \"" + mazeFilePath + "\" contains non-numeric entries.");
                return false;
            }
        }

        BasicTile tile;
        tile.x = strToInt(tokens.at(0));
        tile.y = strToInt(tokens.at(1));
        for (Direction direction : DIRECTIONS) {
            tile.walls[direction] = strToInt(tokens.at(2+direction));
        }
     
        //Order of columns validation
        if(tile.x > columnMax) {
            if(columnCount == 16 && (tile.x - columnMax) == 1) {
               columnMax = tile.x;
               columnCount = 0;
            }
            else {
                print("Error: \"" + mazeFilePath + "\" has wrong number of columns.");
                return false;
            }
        }
        else if(tile.x < columnMax) { 
            print("Error: \"" + mazeFilePath + "\" has columns out of order.");
            return false;
        }
        
        columnCount++;

        //Order of rows validation
        if(tile.y > rowMax) {
            rowMax = tile.y;
        }
        if(tile.y != ++prevRow) {
            //Way to get size without hard coding?
            if(tile.y == 0 && prevRow == 16) {
                prevRow = 0;
            } 
            else {
	        std::cout << tile.x << " " << tile.y << " " << prevRow << "\n";
                print("Error: \"" + mazeFilePath + "\" has rows out of order.");
                return false; 
            }
        }

        // TODO: Much more validation here
    }

    if(columnMax != 15 || columnCount != 16) {
        print("Error: \"" + mazeFilePath + "\" has wrong number of columns.");
        return false;
    }
    else if(rowMax != 15 || prevRow != 15) {
        print("Error: \"" + mazeFilePath + "\" has wrong number of rows.");
        return false;
    }

    
    return true;
}

void MazeFileUtilities::saveMaze(const std::vector<std::vector<BasicTile>>& maze, const std::string& mazeFilePath) {

    // Create the stream
    std::ofstream file(mazeFilePath.c_str());

    // Make sure the file is open
    if (!file.is_open()) {
        print("Error: Unable to save maze to \"" + mazeFilePath + "\".");
        return;
    }

    // Write to the file
    for (int x = 0; x < maze.size(); x += 1) {
        for (int y = 0; y < maze.at(x).size(); y += 1) {
            file << x << " " << y;
            for (Direction direction : DIRECTIONS) {
                file << " " << (maze.at(x).at(y).walls.at(direction) ? 1 : 0);
            }
            file << std::endl;
        }
    }

    file.close();
}

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMaze(const std::string& mazeFilePath) {

    // This should only be called on files that are actually maze files
    ASSERT(MazeFileUtilities::isMazeFile(mazeFilePath));

    // The maze to be returned
    std::vector<std::vector<BasicTile>> maze;

    // First, determine the dimensions of the maze
    std::pair<int, int> mazeSize = getMazeSize(mazeFilePath);
    int width = mazeSize.first;
    int height = mazeSize.second;

    // Then, create the individual tile objects
    for (int x = 0; x < width; x += 1) {
        std::vector<BasicTile> col;
        for (int y = 0; y < height; y += 1) {
            BasicTile tile;
            tile.x = x;
            tile.y = y;
            col.push_back(tile);
        }
        maze.push_back(col);
    }

    // Lastly, read the file and populate the wall values
    std::ifstream file(mazeFilePath.c_str());
    std::string line("");
    while (getline(file, line)) {

        // Put the tokens in a vector
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
             std::back_inserter<std::vector<std::string>>(tokens));

        // Set the values of all of the walls
        for (Direction direction : DIRECTIONS) {
            BasicTile* tile = &maze.at(strToInt(tokens.at(0))).at(strToInt(tokens.at(1)));
            tile->walls[direction] = (1 == strToInt(tokens.at(2 + direction)));
        }
    }
    file.close();
  
    return maze;
}

std::pair<int, int> MazeFileUtilities::getMazeSize(const std::string& mazeFilePath) {

    // Note: This function should only be called on maze files that have been validated.
    // In particular, we assuming that the dimensions of the maze are given in the last,
    // line of the file, which might not be the case if the lines are properly sorted.
    ASSERT(MazeFileUtilities::isMazeFile(mazeFilePath));

    // Read the last, non-empty line of the file
    std::ifstream file(mazeFilePath.c_str());
    std::string line("");
    std::string prevLine("");
    while (getline(file, line)) {
        if (!line.empty()) {
            prevLine = line;
        }
    }
    file.close();

    // Put the tokens in a vector
    std::istringstream iss(prevLine);
    std::vector<std::string> tokens;
    copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
         std::back_inserter<std::vector<std::string>>(tokens));

    // Extract the width and height values, adding 1 because we started counting at 0
    int width = strToInt(tokens.at(0)) + 1;
    int height = strToInt(tokens.at(1)) + 1;
    return std::make_pair(width, height);
}

} //namespace sim
