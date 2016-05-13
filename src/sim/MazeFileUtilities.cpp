#include "MazeFileUtilities.h"

#include <fstream>
#include <iterator>

#include "Assert.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMaze(
        const std::string& mazeFilePath) {

    // Since we don't know anything about the file type ahead of time, we
    // simply brute force try each of the file types until either one succeeds
    // or they all fail

    #define TRY(statement)\
    try {\
        statement;\
    }\
    catch (...) { }

    TRY(return loadMazeFileMapType(mazeFilePath));
    TRY(return loadMazeFileNumType(mazeFilePath));
    throw std::exception();
}

bool MazeFileUtilities::saveMaze(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath,
        MazeFileType mazeFileType) {
    switch (mazeFileType) {
        case MazeFileType::MAP:
            return saveMazeFileMapType(maze, mazeFilePath);
        case MazeFileType::NUM:
            return saveMazeFileNumType(maze, mazeFilePath);
    }
}

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMazeFileMapType(
        const std::string& mazeFilePath) {

    // First, read the entirety of the file
    std::vector<std::string> lines;
    std::ifstream file(mazeFilePath.c_str());
    std::string line("");
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    // The maze to be returned
    std::vector<std::vector<BasicTile>> upsideDownMaze;

    // The character representing a maze post
    char delimiter = '\0';

    // The number of horizontal spaces between columns
    std::vector<int> spaces;

    // Keep track of what row of the maze we're reading
    int rowsFromTopOfMaze = 0;

    // Iterate over all of the lines
    for (int i = 0; i < lines.size(); i += 1) {
        std::string line = lines.at(i);

        // Special case for the first line of the file
        if (i == 0) {

            // The delimiter is taken to be the first character in the file
            delimiter = line.at(0);

            // Tokenize the first line based on the delimiter
            std::vector<std::string> tokens = SimUtilities::tokenize(line, delimiter);
            for (int j = 0; j < tokens.size(); j += 1) {

                // Initialize the columns of the maze
                std::vector<BasicTile> column;
                upsideDownMaze.push_back(column);

                // Keep track of the space between columns
                spaces.push_back(tokens.at(j).size());
            }
        }

        // Extract horizontal wall info, tiles structs don't already exist
        if (line.at(0) == delimiter) {
            int position = 1 + (spaces.at(0) - 1) / 2; // Center of the wall
            for (int j = 0; j < spaces.size(); j += 1) {
                BasicTile tile;
                bool isWall = line.at(position) != ' ';
                tile.walls.at(Direction::NORTH) = isWall;
                upsideDownMaze.at(j).push_back(tile);
                if (0 < rowsFromTopOfMaze) {
                    upsideDownMaze.at(j).at(rowsFromTopOfMaze - 1).walls.at(Direction::SOUTH) = isWall;
                }
                if (j < spaces.size() - 1) {
                    position += 1 + spaces.at(j) / 2; // Position of the next corner
                    position += 1 + (spaces.at(j + 1) - 1) / 2; // Center of the wall
                }
            }
        }

        // Extract vertical wall info, tiles structs already exist
        else if (lines.at(i - 1).at(0) == delimiter) {
            int position = 0;
            for (int j = 0; j <= spaces.size(); j += 1) {
                bool isWall = line.at(position) != ' ';
                if (0 < position) {
                    upsideDownMaze.at(j - 1).at(rowsFromTopOfMaze).walls.at(Direction::EAST) = isWall;
                }
                if (position < line.size() - 1) {
                    upsideDownMaze.at(j).at(rowsFromTopOfMaze).walls.at(Direction::WEST) = isWall;
                }
                if (j < spaces.size()) {
                    position += spaces.at(j) + 1;
                }
            }
            rowsFromTopOfMaze += 1;
        }
    }

    // Strip off of the last extraneous row
    for (int i = 0; i < upsideDownMaze.size(); i += 1) {
        upsideDownMaze.at(i).pop_back();
    }

    // Flip the maze so that it's right side up
    std::vector<std::vector<BasicTile>> rightSideUpMaze;
    for (int i = 0; i < upsideDownMaze.size(); i += 1) {
        std::vector<BasicTile> column;
        for (int j = upsideDownMaze.at(i).size() - 1; j >= 0; j -= 1) {
            column.push_back(upsideDownMaze.at(i).at(j));
        }
        rightSideUpMaze.push_back(column);
    }

    return rightSideUpMaze;
}

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMazeFileNumType(
        const std::string& mazeFilePath) {

    // The maze to be returned
    std::vector<std::vector<BasicTile>> maze;

    // The column to be appended
    std::vector<BasicTile> column;

    // First, read the entirety of the file
    std::vector<std::string> lines;
    std::ifstream file(mazeFilePath.c_str());
    std::string line("");
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    // Iterate over all of the lines
    for (std::string line : lines) {

        // Put the tokens in a vector
        std::vector<std::string> tokens = SimUtilities::tokenize(line);

        // Fill the BasicTile object with the values
        BasicTile tile;
        for (Direction direction : DIRECTIONS) {
            tile.walls.at(direction) =
                (1 == SimUtilities::strToInt(tokens.at(2 + SimUtilities::getDirectionIndex(direction))));
        }

        // If the tile belongs to a new column, append the current column and then empty it
        if (maze.size() < SimUtilities::strToInt(tokens.at(0))) {
            maze.push_back(column);
            column.clear();
        }

        // Always append the current tile to the current column
        column.push_back(tile);
    }

    // Make sure to append the last column
    maze.push_back(column);

    return maze;
}

bool MazeFileUtilities::saveMazeFileMapType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath) {

    // TODO: upforgrabs
    //
    // Write a function that saves the current maze
    // to mazeFilePath in the map format, like:
    //
    //            +---+---+---+---+
    //            |   |           |
    //            +   +---+   +   +
    //            |   |       |   |
    //            +   +   +   +   +
    //            |   |       |   |
    //            +   +---+---+   +
    //            |               |
    //            +---+---+---+---+

    return false;
}

bool MazeFileUtilities::saveMazeFileNumType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath) {

    // Create the stream
    std::ofstream file(mazeFilePath.c_str());

    // Make sure the file is open
    if (!file.is_open()) {
        return false;
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

    // Make sure to close the file
    file.close();

    // Return success
    return true;
}

} //namespace sim
