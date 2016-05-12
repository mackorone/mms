#include "MazeFileUtilities.h"

#include <fstream>
#include <iterator>

#include "Assert.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

std::map<MazeFileType, std::vector<std::string>> MazeFileUtilities::checkMazeFile(
        const std::string& mazeFilePath) {
    return {
        {MazeFileType::BIN, checkMazeFileBinType(mazeFilePath)},
        {MazeFileType::MAP, checkMazeFileMapType(mazeFilePath)},
        {MazeFileType::NUM, checkMazeFileNumType(mazeFilePath)},
    };
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
        default:
            SIM_ASSERT_NEVER_RUNS();
    }
}

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMaze(
        const std::string& mazeFilePath) {
    // TODO: MACK - move this lower, once all of the functions are implemented
    if (checkMazeFileNumType(mazeFilePath).empty()) {
        return loadMazeFileNumType(mazeFilePath);
    }
    if (checkMazeFileBinType(mazeFilePath).empty()) {
        return loadMazeFileBinType(mazeFilePath);
    }
    if (checkMazeFileMapType(mazeFilePath).empty()) {
        return loadMazeFileMapType(mazeFilePath);
    }
    SIM_ASSERT_NEVER_RUNS();
}

std::vector<std::string> MazeFileUtilities::checkMazeFileBinType(
        const std::string& mazeFilePath) {
    // TODO: MACK
    return {"ERROR"};
}

std::vector<std::string> MazeFileUtilities::checkMazeFileMapType(
        const std::string& mazeFilePath) {
    // TODO: MACK
    return {};
}

std::vector<std::string> MazeFileUtilities::checkMazeFileNumType(
        const std::string& mazeFilePath) {

    // List of errors to return
    std::vector<std::string> errors;

    // Definitions:
    //  X-value - The first integer value in a particular line
    //  Y-value - The second integer value in a particular line
    //  Column - A group of one or more lines that share the same X-value value
    //
    // Format requires that:
    //  - The file must exist
    //  - The file must not be empty
    //  - Each line consists of six whitespace separated tokens
    //  - Each of the six tokens are integer values
    //  - The last four tokens must be either 0 or 1
    //  - The lines should be sorted by X-value, and then by Y-value
    //  - The X-value of the first line should be 0
    //  - X-values should never decrease between any two subsequent lines
    //  - X-values should increase by at most 1 between any two subsequent lines
    //  - The Y-value of the first line of each column should be 0
    //  - Y-values should never decrease between any two subsequent lines within a column
    //  - Y-values should increase by at most 1 between any two subsequent lines
    //  - (X-value, Y-value) tuples must be unique
    //
    // Note that the maze does not have to be rectangular to be considered a maze file.

    // TODO: MACK - use QString for string formatting

    // First, make sure we've been given a file
    if (!SimUtilities::isFile(mazeFilePath)) {
        errors.push_back(
            std::string("\"") +
            mazeFilePath +
            "\" is not a file."
        );
        return errors;
    }

    // Create the file object
    std::ifstream file(mazeFilePath.c_str());

    // Error opening file
    if (!file.is_open()) {
        errors.push_back(
            std::string("Could not open \"") +
            mazeFilePath +
            "\" for maze validation."
        );
        return errors;
    }

    // Empty file
    if (file.peek() == std::ifstream::traits_type::eof()) {
        errors.push_back(
            std::string("\"") +
            mazeFilePath +
            "\" is empty."
        );
        return errors;
    }

    std::string line("");
    int lineNum = 0;
    int expectedX = 0;
    int expectedY = 0;

    while (std::getline(file, line)) {

        // Increment the line number
        lineNum += 1;

        // Extract the whitespace separated tokens
        std::vector<std::string> tokens = SimUtilities::tokenize(line);

        // Check to see that there are exactly six entries...
        if (6 != tokens.size()) {
            errors.push_back(
                std::string("\"") +
                mazeFilePath +
                "\" does not contain six entries on each line: line " + 
                std::to_string(lineNum) +
                " contains " +
                std::to_string(tokens.size()) +
                " entries."
            );
            return errors;
        }

        // ... all of which are numeric
        std::vector<int> values;
        for (int i = 0; i < tokens.size(); i += 1) {
            if (!SimUtilities::isInt(tokens.at(i))) {
                errors.push_back(
                    std::string("\"") +
                    mazeFilePath +
                    "\" contains non-numeric entries: the entry" +
                    tokens.at(i) +
                    " on line " +
                    std::to_string(lineNum) +
                    " in position " +
                    std::to_string(i + 1) +
                    " is not numeric."
                );
                return errors;
            }
            else {
                values.push_back(SimUtilities::strToInt(tokens.at(i)));
            }
        }

        // Check the expected X and expected Y. Note that the only time expect a Y-value of zero is the
        // very first line. The "&& expectedY != 0" ensures that the first line must be (0,0).
        bool caseOne = values.at(0) == expectedX     && values.at(1) == expectedY;
        bool caseTwo = values.at(0) == expectedX + 1 && values.at(1) == 0 && expectedY != 0;
        if (caseOne) {
            expectedY += 1;
        }
        else if (caseTwo) {
            expectedX += 1;
            expectedY = 1;
        }
        else {
            errors.push_back(
                std::string("\"") +
                mazeFilePath +
                "\" contains unexpected x and y values of " +
                std::to_string(values.at(0)) +
                " and " +
                std::to_string(values.at(1)) +
                " on line " +
                std::to_string(lineNum) +
                "."
            );
            return errors;
        }

        // Check the wall values to ensure that they're either 0 or 1
        for (int i = 0; i < 4; i += 1) {
            int value = values.at(2 + i);
            if (!(value == 0 || value == 1)) {
                errors.push_back(
                    std::string("\"") +
                    mazeFilePath +
                    "\" contains an invalid value of " +
                    std::to_string(value) +
                    " in position " +
                    std::to_string(2 + i + 1) +
                    " on line " +
                    std::to_string(lineNum) +
                    ". All wall values must be either \"0\" or \"1\"."
                );
                return errors;
            }
        }
    }

    // TODO: MACK - close the file here...

    return {};
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

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMazeFileBinType(
        const std::string& mazeFilePath) {
    // TODO: MACK
    SIM_ASSERT_NEVER_RUNS();
}

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMazeFileMapType(
        const std::string& mazeFilePath) {

    // TODO: MACK - clean this up, make it resilient to any amount of horizontal space

    // This should only be called on files that are actually maze files
    SIM_ASSERT_TR(MazeFileUtilities::checkMazeFileMapType(mazeFilePath).empty());

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

    // This should only be called on files that are actually maze files
    SIM_ASSERT_TR(MazeFileUtilities::checkMazeFileNumType(mazeFilePath).empty());

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

} //namespace sim
