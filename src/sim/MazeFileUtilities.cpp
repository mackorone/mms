#include "MazeFileUtilities.h"

#include <fstream>
#include <cstdint>

#include "SimUtilities.h"
#include "MazeChecker.h"

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

    // We try these in order of increasing permissiveness
    TRY(return loadMazeFileNumType(mazeFilePath));
    TRY(return loadMazeFileMz2Type(mazeFilePath));
    TRY(return loadMazeFileMapType(mazeFilePath));
    TRY(return loadMazeFileMazType(mazeFilePath));

    // I have moved maze file validation into the load routine so that the above issue won't happen
    // anymore. This makes sense becuase the load routine should succesfully load a valid maze
    // before it returns. This would prevent any decoder from incorrectly decoding the file.

    throw std::exception();
}

bool MazeFileUtilities::saveMaze(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath,
        MazeFileType mazeFileType) {
    switch (mazeFileType) {
        case MazeFileType::MAP:
            return saveMazeFileMapType(maze, mazeFilePath);
        case MazeFileType::MAZ:
            return saveMazeFileMazType(maze, mazeFilePath);
        case MazeFileType::MZ2:
            return saveMazeFileMz2Type(maze, mazeFilePath);
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
    file.close(); // The file should be read and closed up here since we might error out

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

    if (!MazeChecker::isValidMaze(rightSideUpMaze)) {
        throw std::exception(); // The load produced an incorrect maze
    }

    return rightSideUpMaze;
}

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMazeFileMazType(
        const std::string& mazeFilePath) {
	
    std::vector<char> characters;
    std::ifstream file(mazeFilePath.c_str());
    char character = ' ';
    while (file.get(character)) {
        characters.push_back(character);
    }
    file.close(); // The file should be read and closed up here since we might error out

    // The maze to be returned
    std::vector<std::vector<BasicTile>> maze;

    // This maze file format is written to only accomodate 16x16 mazes
    // We can hardcode this becuase all the load functions run in try blocks
    for (int x = 0; x < 16; x += 1) {
        std::vector<BasicTile> column;
        for (int y = 0; y < 16; y += 1) {
            int walls = characters.at(x * 16 + y);
            BasicTile tile;
            //Each byte reprsents the walls like this: 'X X X X W S E N'
            tile.walls[Direction::WEST]  = (walls & 1 << 3) != 0;
            tile.walls[Direction::SOUTH] = (walls & 1 << 2) != 0;
            tile.walls[Direction::EAST]  = (walls & 1 << 1) != 0;
            tile.walls[Direction::NORTH] = (walls & 1 << 0) != 0;
            column.push_back(tile);
        }
        maze.push_back(column);
    }

    if (!MazeChecker::isValidMaze(maze)) {
        throw std::exception(); // The load produced an incorrect maze
    }

    return maze;

}

std::vector<std::vector<BasicTile>> MazeFileUtilities::loadMazeFileMz2Type(
        const std::string& mazeFilePath) {

    std::vector<char> characters;
    std::ifstream file(mazeFilePath.c_str());
    char character = ' ';
    while (file.get(character)) {
        characters.push_back(character);
    }
    file.close(); // The file should be read and closed up here since we might error out

    // Reverse the vector order so we can pop back for the next character
    std::reverse(characters.begin(), characters.end());

    // Why oh why can't cant iterators throw exceptions when derefrencing to invalid locations
    // Pop back and get popped element
    auto getPopBack = [](std::vector<char>* v) {if(v->size()==0){throw std::exception();}
                                                      auto e = v->back();
                                                      v->pop_back();
                                                      return e;};

    uint32_t stringLength = (getPopBack(&characters) << 4) +
                             getPopBack(&characters);

    std::string mazeName; // The title is not used, but here if needed
                          // It is a UTF-8 formatted sring

    if (stringLength != 0) {
        while (stringLength != 0) {
            unsigned char character = getPopBack(&characters);
            mazeName += character;
            if (character >> 7 == 0 ||
                character >> 6 == 3) { // 11 in binary
                // This is a utf-8 formated string.  Only decrement the counter
                // if it is the first character of a sequence.
                stringLength--;
            }
        }
    }

    uint32_t width = (getPopBack(&characters) << 24) +
                     (getPopBack(&characters) << 16) +
                     (getPopBack(&characters) << 8) +
                     (getPopBack(&characters));

    uint32_t height = (getPopBack(&characters) << 24) +
                      (getPopBack(&characters) << 16) +
                      (getPopBack(&characters) << 8) +
                      (getPopBack(&characters));
    
    std::vector<std::vector<BasicTile>> maze;

    for (auto x = 0; x < width; x++) {
        std::vector<BasicTile> column;
        for (auto y = 0; y < height; y++) {
            BasicTile tile;
            for (Direction direction : DIRECTIONS) {
                // Make a filled maze so we get the maze border for free
                // and don't need any special logic to make it happen
                tile.walls.insert(std::make_pair(direction, true));
            }
            column.push_back(tile);
        }
        maze.push_back(column);
    }

    int numberOfBits = 0;
    int numberOfBytes = 0;
    unsigned char byte = getPopBack(&characters);

    for (auto y = 0; y < height - 1; y++) {
        for (auto x = 0; x < width; x++) {
            bool wallExists = (byte & 1) == 1;
            byte >>= 1;

            maze.at(x).at(height - 1 - y).walls[Direction::SOUTH] = wallExists;
            maze.at(x).at(height - 1 - y - 1).walls[Direction::NORTH] = wallExists;

            numberOfBits = (numberOfBits + 1) % 8;

            if (numberOfBits == 0) {
                byte = getPopBack(&characters);
                numberOfBytes = (numberOfBytes + 1) % 8; // Add one to the number of bytes
            }
        }
    }

    if (numberOfBytes != 0) {
        for (auto i = 0; i < (7 - numberOfBytes); i++) {
            getPopBack(&characters); // Padding so the number of bytes is a muliple of 8
        }
        numberOfBytes = 0;
    }
    numberOfBits = 0;

    byte = getPopBack(&characters);

    for (auto x = 0; x < width - 1; x++) {
        for (auto y = 0; y < height; y++) {
            bool wallExists = (byte & 1) == 1;
            byte >>= 1;

            maze.at(x).at(height - 1 - y).walls[Direction::EAST] = wallExists;
            maze.at(x + 1).at(height - 1 - y).walls[Direction::WEST] = wallExists;
            
            numberOfBits = (numberOfBits + 1) % 8;

            if (numberOfBits == 0) {
                byte = getPopBack(&characters);
                numberOfBytes = (numberOfBytes + 1) % 8; // Add one to the number of bytes
            }
        }
    }

    if (!MazeChecker::isValidMaze(maze)) {
        throw std::exception(); // The load produced an incorrect maze
    }

    return maze;
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
    file.close(); // The file should be read and closed up here since we might error out

    // Iterate over all of the lines
    for (std::string line : lines) {

        // Put the tokens in a vector
        std::vector<std::string> tokens = SimUtilities::tokenize(line, ' ', true, false);

        // Fill the BasicTile object with the values
        BasicTile tile;
        for (Direction direction : DIRECTIONS) {
            tile.walls.insert(std::make_pair(direction,
                (1 == std::stoi(tokens.at(2 + SimUtilities::getDirectionIndex(direction))))
                // We can't use the sim utilities string to into becuase we have to throw an
                // exception if we fail. The sim utilities throw an assert.
                //
                // We might want to rethink throwing the assert. Why not use the std c++ way
                // of dealing with the error
            ));
        }

        // If the tile belongs to a new column, append the current column and then empty it
        if (maze.size() < std::stoi(tokens.at(0))) {
            maze.push_back(column);
            column.clear();
        }

        // Always append the current tile to the current column
        column.push_back(tile);
    }

    // Make sure to append the last column
    maze.push_back(column);

    if (!MazeChecker::isValidMaze(maze)) {
        throw std::exception(); // The load produced an incorrect maze
    }

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
    std::ofstream file(mazeFilePath.c_str(), std::ios::trunc);

    // Make sure the file is open
    if (!file.is_open()) {
        return false;
    }

    // Write to the file
    for (std::string line : rightSideUpLines) {
        file << line << '\n'; // std::endl flushes the buffer everytime its called
    }

    // Make sure to close the file
    file.close();

    // Return success
    return true;
}

bool MazeFileUtilities::saveMazeFileMazType(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath) {

    if (maze.size() != 16) {
        return false; // We only support 16x16 mazes
    }

    if (maze.at(0).size() != 16) {
        // Lets assume the maze is at least rectangular. I don't know if it cannot be

        return false; // We only support 16x16 mazes
    }

    // Create the stream
    std::ofstream file(mazeFilePath.c_str(), std::ios::trunc | std::ios::binary);
    file.imbue(std::locale::classic());

    // Make sure the file is open
    if (!file.is_open()) {
        return false;
    }

    // We hardcode values becuase we specifically checked that the maze
    // was 16x16 above.
    for (auto x = 0; x < 16; x++) {
        for (auto y = 0; y < 16; y++) {
            BasicTile tile = maze.at(x).at(y);
            //Each byte reprsents the walls like this: 'X X X X W S E N'
            int representation = (tile.walls[Direction::WEST]  << 3) +
                                 (tile.walls[Direction::SOUTH] << 2) +
                                 (tile.walls[Direction::EAST]  << 1) +
                                 (tile.walls[Direction::NORTH] << 0);

            file.put(representation);
        }
    }

    // Make sure to close the file
    file.close();

    // Return success
    return true;
}

bool MazeFileUtilities::saveMazeFileMz2Type(
        const std::vector<std::vector<BasicTile>>& maze,
        const std::string& mazeFilePath) {

    // Create the stream
    std::ofstream file(mazeFilePath.c_str(), std::ios::trunc | std::ios::binary);
    file.imbue(std::locale::classic());

    // Make sure the file is open
    if (!file.is_open()) {
        return false;
    }

    file.put(0);  // Hardcoded name is: 'Auto Generated Maze' (length: 19)
    file.put(19); // I am assuming that the length is encoded in 16 bits.
                  // The spec is unclear
    
    file << "Auto Generated Maze";

    uint32_t width = maze.size();
    uint32_t height = 0;

    if (maze.size() > 0) {
        height = maze.at(0).size();
        if (height == 0)
            return false; // I don't think this should be allowed?
    }
    else
        return false; // I don't think this should be allowed?

    file.put(width >> 24); // width
    file.put(width >> 16);
    file.put(width >> 8);
    file.put(width);

    file.put(height >> 24); // height
    file.put(height >> 16);
    file.put(height >> 8);
    file.put(height);

    int numberOfBits = 0;
    int numberOfBytes = 0;
    unsigned char toWrite = 0;

    for (auto y = 0; y < height - 1; y++) {
        for (auto x = 0; x < width; x++) {
            BasicTile tile = maze.at(x).at(height - 1 - y);

            toWrite >>= 1;
            if (tile.walls[Direction::SOUTH] == true) {
                toWrite |= 1 << 7; // Set the MSB bit as one
            }
            numberOfBits = (numberOfBits + 1) % 8;

            if (numberOfBits == 0) {
                file.put(toWrite); // Write the byte when it is full
                numberOfBytes = (numberOfBytes + 1) % 8; // Add one to the number of bytes
            }
        }
    }

    if (numberOfBits != 0) {
        file.put(toWrite >> (8 - numberOfBits));
        numberOfBits = 0; // Write the last byte out 
    }
    
    if (numberOfBytes != 0) {
        for (auto i = 0; i < (7 - numberOfBytes); i++) {
            file.put(0); // Padding so the number of bytes is a muliple of 8
        }
        numberOfBytes = 0;
    }

    for (auto x = 0; x < width - 1; x++) {
        for (auto y = 0; y < height; y++) {
            BasicTile tile = maze.at(x).at(height - 1 - y);

            toWrite >>= 1;
            if (tile.walls[Direction::EAST] == true) {
                toWrite |= 1 << 7; // Set the MSB bit as one
            }
            numberOfBits = (numberOfBits + 1) % 8;

            if (numberOfBits == 0) {
                file.put(toWrite); // Write the byte when it is full
                numberOfBytes = (numberOfBytes + 1) % 8; // Add one to the number of bytes
            }
        }
    }

    if (numberOfBits != 0) {
        file.put(toWrite >> (8 - numberOfBits));
    }

    if (numberOfBytes != 0) {
        for (auto i = 0; i < (7 - numberOfBytes); i++) {
            file.put(0); // Padding so the number of bytes is a muliple of 8
        }
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
    std::ofstream file(mazeFilePath.c_str(), std::ios::trunc);

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
            file << "\n"; // std::endl flushes the buffer everytime its called
        }
    }

    // Make sure to close the file
    file.close();

    // Return success
    return true;
}

} //namespace sim
