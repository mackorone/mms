#include "MazeFileUtilities.h"

#include <QChar>
#include <QFile>
#include <QString>

// TODO: MACK - convert to Qt after Tomasz is done fixing
#include <fstream>
#include <cstdint>
#include "Logging.h"
#include "SimUtilities.h"

#include "MazeChecker.h"

namespace sim {

BasicMaze MazeFileUtilities::load(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        // TODO: MACK - document what this throws
        throw std::exception();
    }
    return loadBytes(file.readAll());
}

BasicMaze MazeFileUtilities::loadBytes(const QByteArray& bytes) {

    // Since we don't know anything about the file type ahead of time, we
    // simply brute force try each of the file types until either one succeeds
    // or they all fail

    // TODO: MACK - what should be validated here?
    #define TRY(expression, validate)\
    try {\
        BasicMaze maze = expression;\
        if (validate && !MazeChecker::isValidMaze(maze).first) {\
            throw std::exception();\
        }\
        return maze;\
    }\
    catch (...) { }

    // TODO: MACK - We shouldn't print out warning messages here, I don't think
    TRY(deserializeMazType(bytes), true);
    TRY(deserializeMz2Type(bytes), true);
    TRY(deserializeNumType(bytes), true);
    TRY(deserializeMapType(bytes), false);
    throw std::exception();
}

void MazeFileUtilities::save(
    const BasicMaze& maze,
    const QString& path,
    MazeFileType type) {

    // TODO: MACK - should throw exception
    QByteArray bytes;
    switch (type) {
        case MazeFileType::MAP:
            bytes = serializeMapType(maze);
        case MazeFileType::MAZ:
            bytes = serializeMazType(maze);
        case MazeFileType::MZ2:
            bytes = serializeMz2Type(maze);
        case MazeFileType::NUM:
            bytes = serializeNumType(maze);
        default:
            // TODO: MACK
            SimUtilities::quit();
    }
    // TODO: MACK - write to file
}

BasicMaze MazeFileUtilities::deserializeMapType(const QByteArray& bytes) {

    // TODO: MACK
    // +++
    // |
    // +++
    // +++
    // Doesn't load

    // First, convert the bytes to lines
    QStringList lines = QString(bytes).trimmed().split("\n");

    // The maze to be returned
    BasicMaze upsideDownMaze;

    // The character representing a maze post
    QChar delimiter('\0');

    // The number of horizontal spaces between columns
    QVector<int> spaces;

    // Keep track of what row of the maze we're reading
    int rowsFromTopOfMaze = -1;

    // Iterate over all of the lines
    for (int i = 0; i < lines.size(); i += 1) {
        QString line = lines.at(i);

        // Special case for the first line of the file
        if (i == 0) {
            delimiter = line.at(0);
            QStringList tokens = line.split(delimiter, QString::SkipEmptyParts);
            for (int j = 0; j < tokens.size(); j += 1) {
                QVector<BasicTile> column;
                upsideDownMaze.push_back(column);
                spaces.push_back(tokens.at(j).size());
            }
        }

        // Extract horizontal wall info, tiles structs don't already exist
        if (line.at(0) == delimiter) {
            rowsFromTopOfMaze += 1;
            int position = (spaces.at(0) + 1) / 2; // Center of the wall
            for (int j = 0; j < spaces.size(); j += 1) {
                if (line.size() <= position) {
                    break;
                }
                BasicTile tile;
                for (Direction direction : DIRECTIONS) {
                    tile.insert(direction, false);
                }
                upsideDownMaze[j].push_back(tile);
                if (0 < spaces.at(j)) {
                    bool isWall = line.at(position) != ' ';
                    upsideDownMaze[j][rowsFromTopOfMaze][Direction::NORTH] = isWall;
                    if (0 < rowsFromTopOfMaze) {
                        upsideDownMaze[j][rowsFromTopOfMaze - 1][Direction::SOUTH] = isWall;
                    }
                }
                if (j < spaces.size() - 1) {
                    position += 1 + spaces.at(j) / 2; // Position of the next corner
                    position += (spaces.at(j + 1) + 1) / 2; // Center of the wall
                }
            }
        }

        // Extract vertical wall info, tiles structs already exist
        else if (lines.at(i - 1).at(0) == delimiter) {
            int position = 0;
            for (int j = 0; j <= spaces.size(); j += 1) {
                if (line.size() <= position) {
                    break;
                }
                bool isWall = line.at(position) != ' ';
                if (0 < j) {
                    upsideDownMaze[j - 1][rowsFromTopOfMaze][Direction::EAST] = isWall;
                }
                if (j < spaces.size()) {
                    upsideDownMaze[j][rowsFromTopOfMaze][Direction::WEST] = isWall;
                    position += spaces.at(j) + 1;
                }
            }
        }
    }

    // Strip off of the last extraneous row
    for (int i = 0; i < upsideDownMaze.size(); i += 1) {
        upsideDownMaze[i].pop_back();
    }

    // Flip the maze so that it's right side up
    BasicMaze rightSideUpMaze;
    for (int i = 0; i < upsideDownMaze.size(); i += 1) {
        QVector<BasicTile> column;
        for (int j = upsideDownMaze.at(i).size() - 1; j >= 0; j -= 1) {
            column.push_back(upsideDownMaze.at(i).at(j));
        }
        rightSideUpMaze.push_back(column);
    }

    return rightSideUpMaze;
}

BasicMaze MazeFileUtilities::deserializeMazType(const QByteArray& bytes) {

    // TODO: MACK Convert this to use Qt
    QVector<char> characters;
    for (int i = 0; i < bytes.size(); i += 1) {
        characters.push_back(bytes.at(i));
    }
	
    // The maze to be returned
    BasicMaze maze;

    // This maze file format is written to only accomodate 16x16 mazes
    // We can hardcode this becuase all the load functions run in try blocks
    for (int x = 0; x < 16; x += 1) {
        QVector<BasicTile> column;
        for (int y = 0; y < 16; y += 1) {
            int walls = characters.at(x * 16 + y);
            BasicTile tile;
            //Each byte reprsents the walls like this: 'X X X X W S E N'
            tile[Direction::WEST]  = (walls & 1 << 3) != 0;
            tile[Direction::SOUTH] = (walls & 1 << 2) != 0;
            tile[Direction::EAST]  = (walls & 1 << 1) != 0;
            tile[Direction::NORTH] = (walls & 1 << 0) != 0;
            column.push_back(tile);
        }
        maze.push_back(column);
    }

    return maze;
}

BasicMaze MazeFileUtilities::deserializeMz2Type(const QByteArray& bytes) {

    // TODO: Convert this to use Qt
    QVector<char> characters;
    for (int i = 0; i < bytes.size(); i += 1) {
        characters.push_back(bytes.at(i));
    }

    // Reverse the vector order so we can pop back for the next character
    std::reverse(characters.begin(), characters.end());

    // Why oh why can't cant iterators throw exceptions when derefrencing to invalid locations
    // Pop back and get popped element
    auto getPopBack = [](QVector<char>* v) {if(v->size()==0){throw std::exception();}
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
    
    // Let's make sure we do not read a massive size and go on forerver
    if (width > 256 || height > 256) {
        throw std::exception();
    }

    BasicMaze maze;

    for (auto x = 0; x < width; x++) {
        QVector<BasicTile> column;
        for (auto y = 0; y < height; y++) {
            BasicTile tile;
            for (Direction direction : DIRECTIONS) {
                // Make a filled maze so we get the maze border for free
                // and don't need any special logic to make it happen
                tile.insert(direction, true);
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

            maze[x][height - 1 - y][Direction::SOUTH] = wallExists;
            maze[x][height - 1 - y - 1][Direction::NORTH] = wallExists;

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

            maze[x][height - 1 - y][Direction::EAST] = wallExists;
            maze[x + 1][height - 1 - y][Direction::WEST] = wallExists;
            
            numberOfBits = (numberOfBits + 1) % 8;

            if (numberOfBits == 0) {
                byte = getPopBack(&characters);
                numberOfBytes = (numberOfBytes + 1) % 8; // Add one to the number of bytes
            }
        }
    }

    return maze;
}

BasicMaze MazeFileUtilities::deserializeNumType(const QByteArray& bytes) {

    /*
    // The maze to be returned
    BasicMaze maze;

    // The column to be appended
    QVector<BasicTile> column;

    // First, read the entirety of the file
    QVector<QString> lines;
    std::ifstream file(mazeFilePath.c_str());
    QString line("");
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close(); // The file should be read and closed up here since we might error out

    // Iterate over all of the lines
    for (QString line : lines) {

        // Put the tokens in a vector
        QVector<QString> tokens = SimUtilities::tokenize(line, ' ', true, false);

        // Fill the BasicTile object with the values
        BasicTile tile;
        for (Direction direction : DIRECTIONS) {
            tile.insert({direction,
                (1 == std::stoi(tokens.at(2 + SimUtilities::getDirectionIndex(direction))))
                // We can't use the sim utilities string to into becuase we have to throw an
                // exception if we fail. The sim utilities throw an assert.
                //
                // We might want to rethink throwing the assert. Why not use the std c++ way
                // of dealing with the error
            });
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
    */
    throw std::exception();
}

QByteArray MazeFileUtilities::serializeMapType(const BasicMaze& maze) {

    /*
    // The characters to use in the file
    char post = '+';
    char space = ' ';
    char vertical = '|';
    char horizontal = '-';

    // A blank line, and a list of all lines to be written
    QString blankLine(4 * maze.size() + 1, space);
    QVector<QString> upsideDownLines {blankLine};

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
            if (maze.at(i).at(j).at(Direction::NORTH)) {
                for (int k = 0; k < 3; k += 1) {
                    upsideDownLines.at(up).at(left + 1 + k) = horizontal;
                }
            }
            if (maze.at(i).at(j).at(Direction::SOUTH)) {
                for (int k = 0; k < 3; k += 1) {
                    upsideDownLines.at(down).at(left + 1 + k) = horizontal;
                }
            }
            if (maze.at(i).at(j).at(Direction::EAST)) {
                upsideDownLines.at(down + 1).at(right) = vertical;
            }
            if (maze.at(i).at(j).at(Direction::WEST)) {
                upsideDownLines.at(down + 1).at(left) = vertical;
            }
        }
    }

    // Flip the lines so that they're right side up
    QVector<QString> rightSideUpLines;
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
    for (QString line : rightSideUpLines) {
        file << line << '\n'; // std::endl flushes the buffer everytime its called
    }

    // Make sure to close the file
    file.close();

    // Return success
    return true;
    */
    throw std::exception();
}

QByteArray MazeFileUtilities::serializeMazType(const BasicMaze& maze) {

    /*
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
            int representation = (tile[Direction::WEST]  << 3) +
                                 (tile[Direction::SOUTH] << 2) +
                                 (tile[Direction::EAST]  << 1) +
                                 (tile[Direction::NORTH] << 0);

            file.put(representation);
        }
    }

    // Make sure to close the file
    file.close();

    // Return success
    return true;
    */
    throw std::exception();
}

QByteArray MazeFileUtilities::serializeMz2Type(const BasicMaze& maze) {

    /*
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
            if (tile[Direction::SOUTH] == true) {
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
            if (tile[Direction::EAST] == true) {
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
    */
    throw std::exception();
}

QByteArray MazeFileUtilities::serializeNumType(const BasicMaze& maze) {

    /*
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
                file << " " << (maze.at(x).at(y).at(direction) ? 1 : 0);
            }
            file << "\n"; // std::endl flushes the buffer everytime its called
        }
    }

    // Make sure to close the file
    file.close();

    // Return success
    return true;
    */
    throw std::exception();
}

} //namespace sim
