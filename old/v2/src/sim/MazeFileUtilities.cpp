#include "MazeFileUtilities.h"

#include <fstream>
#include <iostream> // TODO: MACK - remove this

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

    // NUM is a little stricter, so we try that first
    TRY(return loadMazeFileNumType(mazeFilePath));
    TRY(return loadMazeFileMapType(mazeFilePath));
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

    // First, read all of the non-empty lines
    std::vector<std::string> lines;
    std::ifstream file(mazeFilePath.c_str());
    std::string line("");
    while (getline(file, line)) {
        if (0 < SimUtilities::trim(line).size()) {
            lines.push_back(line);
        }
    }
    file.close();

    // The maze to be returned
    std::vector<std::vector<BasicTile>> upsideDownMaze;

    // The character representing a maze post
    char delimiter = '\0';

    // The number of horizontal spaces between columns
    std::vector<int> spaces;

    // Keep track of what row of the maze we're reading
    int rowsFromTopOfMaze = -1;

    // Iterate over all of the lines
    for (int i = 0; i < lines.size(); i += 1) {
        std::string line = lines.at(i);

        // Special case for the first line of the file
        if (i == 0) {
            delimiter = line.at(0);
            std::vector<std::string> tokens = SimUtilities::tokenize(line, delimiter, false, false);
            for (int j = 0; j < tokens.size(); j += 1) {
                std::vector<BasicTile> column;
                upsideDownMaze.push_back(column);
                spaces.push_back(tokens.at(j).size());
            }
        }

        // Extract horizontal wall info, tiles structs don't already exist
        if (0 < line.size() && line.at(0) == delimiter) {
            rowsFromTopOfMaze += 1;
            int position = (spaces.at(0) + 1) / 2; // Center of the wall
            for (int j = 0; j < spaces.size(); j += 1) {
                BasicTile tile;
                for (Direction direction : DIRECTIONS) {
                    tile.walls.insert(std::make_pair(direction, false));
                }
                upsideDownMaze.at(j).push_back(tile);
                if (0 < spaces.at(j)) {
                    bool isWall = line.at(position) != ' ';
                    upsideDownMaze.at(j).at(rowsFromTopOfMaze).walls.at(Direction::NORTH) = isWall;
                    if (0 < rowsFromTopOfMaze) {
                        upsideDownMaze.at(j).at(rowsFromTopOfMaze - 1).walls.at(Direction::SOUTH) = isWall;
                    }
                }
                if (j < spaces.size() - 1) {
                    position += 1 + spaces.at(j) / 2; // Position of the next corner
                    position += (spaces.at(j + 1) + 1) / 2; // Center of the wall
                }
            }
        }

        // Extract vertical wall info, tiles structs already exist
        else if (0 < lines.at(i - 1).size() && lines.at(i - 1).at(0) == delimiter) {
            int position = 0;
            for (int j = 0; j <= spaces.size(); j += 1) {
                if (line.size() <= position) {
                    break;
                }
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
        std::vector<std::string> tokens = SimUtilities::tokenize(line, ' ', true, false);

        // Fill the BasicTile object with the values
        BasicTile tile;
        for (Direction direction : DIRECTIONS) {
            tile.walls.insert(std::make_pair(direction,
                (1 == SimUtilities::strToInt(tokens.at(2 + SimUtilities::getDirectionIndex(direction))))
            ));
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

    // The characters to use in the file
    char post = '+';
    char space = ' ';
    char vertical = '|';
    char horizontal = '-';

    // A blank line, and a list of all lines to be written
    std::string blankLine(4 * maze.size() + 1, space);
    std::vector<std::string> upsideDownLines {blankLine};

    // For all tiles in the maze
    for (int i = 0; i < maze.size(); i += 1) {
        for (int j = 0; j < maze.at(i).size(); j += 1) {

            // Insert more lines if necessary
            if (upsideDownLines.size() <= 2 * j + 1) {
                upsideDownLines.push_back(blankLine);
                upsideDownLines.push_back(blankLine);
            }

            // Insert posts at the boundaries
            int left = 4 * i;
            int right = 4 * (i + 1);
            int up = 2 * (j + 1);
            int down = 2 * j;
            upsideDownLines.at(down).at(left) = post;
            upsideDownLines.at(down).at(right) = post;
            upsideDownLines.at(up).at(left) = post;
            upsideDownLines.at(up).at(right) = post;

            // Insert walls if they exist
            if (maze.at(i).at(j).walls.at(Direction::NORTH)) {
                for (int k = 0; k < 3; k += 1) {
                    upsideDownLines.at(up).at(left + 1 + k) = horizontal;
                }
            }
            if (maze.at(i).at(j).walls.at(Direction::SOUTH)) {
                for (int k = 0; k < 3; k += 1) {
                    upsideDownLines.at(down).at(left + 1 + k) = horizontal;
                }
            }
            if (maze.at(i).at(j).walls.at(Direction::EAST)) {
                upsideDownLines.at(down + 1).at(right) = vertical;
            }
            if (maze.at(i).at(j).walls.at(Direction::WEST)) {
                upsideDownLines.at(down + 1).at(left) = vertical;
            }
        }
    }

    // Flip the lines so that they're right side up
    std::vector<std::string> rightSideUpLines;
    for (int i = upsideDownLines.size() - 1; i >= 0; i -= 1) {
        rightSideUpLines.push_back(upsideDownLines.at(i));
    }

    // Create the stream
    std::ofstream file(mazeFilePath.c_str());

    // Make sure the file is open
    if (!file.is_open()) {
        return false;
    }

    // Write to the file
    for (std::string line : rightSideUpLines) {
        file << line << std::endl;
    }

    // Make sure to close the file
    file.close();

    // Return success
    return true;
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
