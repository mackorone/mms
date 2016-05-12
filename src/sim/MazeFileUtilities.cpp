#include "MazeFileUtilities.h"

#include <fstream>
#include <iterator>

#include "Assert.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMaze(
        const std::string& mazeFilePath) {

    #define TRY(statement)\
    try {\
        statement;\
    }\
    catch (...) { }

    TRY(return loadMazeFileBinType(mazeFilePath));
    TRY(return loadMazeFileMapType(mazeFilePath));
    TRY(return loadMazeFileNumType(mazeFilePath));
    throw std::exception();
}

void MazeFileUtilities::saveMaze(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath,
        MazeFileType mazeFileType) {
    switch (mazeFileType) {
        case MazeFileType::BIN:
            saveMazeFileBinType(maze, mazeFilePath);
            break;
        case MazeFileType::MAP:
            saveMazeFileMapType(maze, mazeFilePath);
            break;
        case MazeFileType::NUM:
            saveMazeFileNumType(maze, mazeFilePath);
            break;
    }
}

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMazeFileBinType(
        const std::string& mazeFilePath) {
    // TODO: MACK
    throw std::exception();
}

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMazeFileMapType(
        const std::string& mazeFilePath) {

    // TODO: MACK - clean this up, make it resilient to any amount of horizontal space

    // The maze to be returned
    std::vector<std::vector<BasicTile>> upsideDownMaze;

    // Keep track of what row we're reading in
    int rowsFromTop = 0;

    // Read the file and populate the wall values
    std::ifstream file(mazeFilePath.c_str());
    std::string line("");
    while (getline(file, line)) {

        // Put the tokens in a vector
        std::vector<std::string> tokens = SimUtilities::tokenize(line, '+');

        // If the maze is empty, this is the first line
        if (upsideDownMaze.empty()) {
            for (int i = 0; i < tokens.size(); i += 1) {
                std::vector<BasicTile> column;
                upsideDownMaze.push_back(column);
            }
        }

        // Extract horizontal wall info, tiles don't already exist
        if (tokens.size() == upsideDownMaze.size()) {
            for (int j = 2; j < line.size(); j += 4) {
                BasicTile tile;
                bool isWall = line.at(j) != ' ';
                tile.walls.insert(std::make_pair(Direction::NORTH, isWall));
                upsideDownMaze.at((j - 2) / 4).push_back(tile);
                if (0 < rowsFromTop) {
                    upsideDownMaze.at((j - 2) / 4).at(rowsFromTop - 1).walls.insert(std::make_pair(Direction::SOUTH, isWall));
                }
            }
        }

        // Extract vertical wall info, tiles already exist
        else {
            for (int j = 0; j < line.size(); j += 4) {
                bool isWall = line.at(j) != ' ';
                if (0 < j) {
                    upsideDownMaze.at((j - 4) / 4).at(rowsFromTop).walls.insert(std::make_pair(Direction::EAST, isWall));
                }
                if (j < line.size() - 1) {
                    upsideDownMaze.at(j / 4).at(rowsFromTop).walls.insert(std::make_pair(Direction::WEST, isWall));
                }
            }
            rowsFromTop += 1;
        }
    }

    // Remember to close the file
    file.close();

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

    // Read the file and populate the wall values
    std::ifstream file(mazeFilePath.c_str());
    std::string line("");
    while (getline(file, line)) {

        // Put the tokens in a vector
        std::vector<std::string> tokens = SimUtilities::tokenize(line);

        // Fill the BasicTile object with the values
        BasicTile tile;
        for (Direction direction : DIRECTIONS) {
            tile.walls.insert(std::make_pair(direction,
                (1 == SimUtilities::strToInt(tokens.at(2 + SimUtilities::getDirectionIndex(direction))))));
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

    // Remember to close the file
    file.close();
  
    return maze;
}

void MazeFileUtilities::saveMazeFileBinType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath) {
    // TODO: MACK
    SIM_ASSERT_NEVER_RUNS();
}

void MazeFileUtilities::saveMazeFileMapType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath) {
    // TODO: MACK
    SIM_ASSERT_NEVER_RUNS();
}

void MazeFileUtilities::saveMazeFileNumType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath) {

    // Create the stream
    std::ofstream file(mazeFilePath.c_str());

    // Make sure the file is open
    if (!file.is_open()) {
        L()->warn("Unable to save maze to \"%v\".", mazeFilePath);
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

    // Make sure to close the file
    file.close();
}

} //namespace sim
