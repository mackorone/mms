#include "Maze.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QProcess>
#include <QString>
#include <QQueue>

#include "Assert.h"
#include "Directory.h"
#include "Logging.h"
#include "MazeChecker.h"
#include "MazeFileType.h"
#include "MazeFileUtilities.h"
#include "Param.h"
#include "SimUtilities.h"
#include "Tile.h"

namespace sim {

Maze::Maze() {
    
    BasicMaze basicMaze;

    if (P()->useMazeFile()) {
        // TODO: MACK - clean this up (the file existence check should be in the utility class)
        QString mazeFilePath = Directory::get()->getResMazeDirectory() + P()->mazeFile();
        try {
            basicMaze = MazeFileUtilities::load(mazeFilePath);
        }
        catch (...) {
            QString reason = (
                SimUtilities::isFile(mazeFilePath) ?
                "invalid format" : "file doesn't exist");
            qCritical()
                << "Unable to initialize maze from file \"" << mazeFilePath
                << "\": " << reason << ".";
            SimUtilities::quit();
        }
    }
    else {
        // TODO: MACK - refactor this logic elsewhere
        QDir mazeAlgosDir(Directory::get()->getSrcMazeAlgosDirectory());
        mazeAlgosDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        mazeAlgosDir.setSorting(QDir::Name | QDir::QDir::IgnoreCase);
        QStringList algos = mazeAlgosDir.entryList();

        // Check to see if there is some directory with the given name
        QString selectedMazeAlgo(P()->mazeAlgorithm());
        if (!algos.contains(selectedMazeAlgo)) {
             qCritical()
                << "\"" << P()->mazeAlgorithm() << "\" is not a valid maze"
                << " algorithm.";
             SimUtilities::quit();
        }

        // Get all files in the directory, recursively
        QDir selectedMazeAlgoDir(mazeAlgosDir);
        selectedMazeAlgoDir.cd(selectedMazeAlgo);
        selectedMazeAlgoDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
        QDirIterator iterator(selectedMazeAlgoDir, QDirIterator::Subdirectories);
        QStringList relativePaths;
        QStringList absolutePaths;
        while (iterator.hasNext()) {
            iterator.next();
            relativePaths << iterator.fileName();
            absolutePaths << iterator.filePath();
        }

        // Deduce whether or not it's a C++ or Python algo
        QStringList args;
        if (relativePaths.contains(QString("Main.cpp"))) {
            // TODO: MACK args
            SimUtilities::quit();
        }
        else if (relativePaths.contains(QString("Main.py"))) {
            args << selectedMazeAlgoDir.absolutePath() + QString("/Main.py");
            args << QString::number(P()->generatedMazeWidth());
            args << QString::number(P()->generatedMazeHeight());
            for (int i = 0; i < args.size(); i += 1) {
                qInfo() << args.at(i);
            }
        }
        else {
            qCritical()
                << "No \"Main.{py,cpp}\" found in \""
                << selectedMazeAlgoDir.absolutePath() << "\"";
            SimUtilities::quit();
        }

        QProcess process; // TODO: MACK - pass in parent here
        process.start("python", args);
        process.waitForFinished();
        QByteArray bytes = process.readAll();
        basicMaze = MazeFileUtilities::loadBytes(bytes);
    }

    // Check to see if it's a valid maze
    m_isValidMaze = MazeChecker::isValidMaze(basicMaze).first;
    if (!m_isValidMaze) {
        qWarning()
            << "The maze failed validation. The mouse algorithm will not"
            << " execute.";
    }

    // Optionally save the maze
    if (!P()->useMazeFile() && P()->saveGeneratedMaze()) {
        MazeFileType type = STRING_TO_MAZE_FILE_TYPE.value(P()->generatedMazeType());
        QString mazeFilePath = Directory::get()->getResMazeDirectory() +
            P()->generatedMazeFile() + MAZE_FILE_TYPE_TO_SUFFIX.value(type);
        // TODO: MACK
        bool success = false; // MazeFileUtilities::save(basicMaze, mazeFilePath, type);
        if (success) {
            qInfo() << "Maze saved to \"" << mazeFilePath << "\".";
        }
        else {
            qWarning() << "Unable to save maze to \"" << mazeFilePath << "\".";
        }
    }

    // Mirror and rotate the maze
    if (P()->mazeMirrored()) {
        basicMaze = mirrorAcrossVertical(basicMaze);
        qInfo() << "Mirroring the maze across the vertical.";
    }
    for (int i = 0; i < P()->mazeRotations(); i += 1) {
        basicMaze = rotateCounterClockwise(basicMaze);
        qInfo() << "Rotating the maze counter-clockwise (" << i + 1 << ").";
    }

    // Then, store whether or not the maze is an official maze
    m_isOfficialMaze = m_isValidMaze && MazeChecker::isOfficialMaze(basicMaze).first;
    if (m_isValidMaze && !m_isOfficialMaze) {
        qWarning() << "The maze did not pass the \"is official maze\" tests.";
    }

    // Load the maze given by the maze generation algorithm
    m_maze = initializeFromBasicMaze(basicMaze);
}

int Maze::getWidth() const {
    return m_maze.size();
}

int Maze::getHeight() const {
    return (m_maze.size() > 0 ? m_maze.at(0).size() : 0);
}

bool Maze::withinMaze(int x, int y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

Tile* Maze::getTile(int x, int y) {
    SIM_ASSERT_TR(withinMaze(x, y));
    return &m_maze[x][y];
}

const Tile* Maze::getTile(int x, int y) const {
    SIM_ASSERT_TR(withinMaze(x, y));
    return &m_maze.at(x).at(y);
}

bool Maze::isValidMaze() const {
    return m_isOfficialMaze;
}

bool Maze::isOfficialMaze() const {
    return m_isOfficialMaze;
}

bool Maze::isCenterTile(int x, int y) const {
    return MazeChecker::getCenterTiles(getWidth(), getHeight()).contains({x, y});
}

QVector<QVector<Tile>> Maze::initializeFromBasicMaze(const BasicMaze& basicMaze) {
    // TODO: MACK - assert valid here
    QVector<QVector<Tile>> maze;
    for (int x = 0; x < basicMaze.size(); x += 1) {
        QVector<Tile> column;
        for (int y = 0; y < basicMaze.at(x).size(); y += 1) {
            Tile tile;
            tile.setPos(x, y);
            for (Direction direction : DIRECTIONS) {
                tile.setWall(direction, basicMaze.at(x).at(y).value(direction));
            }
            tile.initPolygons(basicMaze.size(), basicMaze.at(x).size());
            column.push_back(tile);
        }
        maze.push_back(column);
    }
    maze = setTileDistances(maze);
    return maze;
}

BasicMaze Maze::mirrorAcrossVertical(const BasicMaze& basicMaze) {
    static QMap<Direction, Direction> verticalOpposites {
        {Direction::NORTH, Direction::NORTH},
        {Direction::EAST, Direction::WEST},
        {Direction::SOUTH, Direction::SOUTH},
        {Direction::WEST, Direction::EAST},
    };
    // TODO: MACK - test this
    BasicMaze mirrored;
    for (int x = 0; x < basicMaze.size(); x += 1) {
        QVector<BasicTile> column;
        for (int y = 0; y < basicMaze.at(x).size(); y += 1) {
            BasicTile tile;
            for (Direction direction : DIRECTIONS) {
                tile.insert(
                    direction,
                    basicMaze
                        .at(basicMaze.size() - 1 - x)
                        .at(y)
                        .value(verticalOpposites.value(direction))
                );
            }
            column.push_back(tile);
        }
        mirrored.push_back(column);
    }
    return mirrored; 
}

BasicMaze Maze::rotateCounterClockwise(const BasicMaze& basicMaze) {
    BasicMaze rotated;
    for (int x = 0; x < basicMaze.size(); x += 1) {
        QVector<BasicTile> row;
        for (int y = basicMaze.at(x).size() - 1; y >= 0; y -= 1) {
            BasicTile tile;
            int rotatedTileX = basicMaze.at(x).size() - 1 - y;
            tile.insert(Direction::NORTH, basicMaze.at(x).at(y).value(Direction::EAST));
            tile.insert(Direction::EAST, basicMaze.at(x).at(y).value(Direction::SOUTH));
            tile.insert(Direction::SOUTH, basicMaze.at(x).at(y).value(Direction::WEST));
            tile.insert(Direction::WEST, basicMaze.at(x).at(y).value(Direction::NORTH));
            if (rotated.size() <= rotatedTileX) {
                rotated.push_back({tile});
            }
            else {
                rotated[rotatedTileX].push_back(tile);
            }
        }
    }
    return rotated;
}

QVector<QVector<Tile>> Maze::setTileDistances(QVector<QVector<Tile>> maze) {

    // TODO: MACK - dedup some of this with hasNoInaccessibleLocations

    // The maze is guarenteed to be nonempty and rectangular
    int width = maze.size();
    int height = maze.at(0).size();

    // Helper lambda for retrieving and adjacent tile if one exists, nullptr if not
    // TODO: MACK - this should be in maze utilities too
    auto getNeighbor = [&maze, &width, &height](int x, int y, Direction direction) {
        switch (direction) {
            case Direction::NORTH:
                return (y < height - 1 ? &maze[x][y + 1] : nullptr);
            case Direction::EAST:
                return (x < width - 1 ? &maze[x + 1][y] : nullptr);
            case Direction::SOUTH:
                return (0 < y ? &maze[x][y - 1] : nullptr);
            case Direction::WEST:
                return (0 < x ? &maze[x - 1][y] : nullptr);
        }
    };

    // Determine all of the center tiles
    // TODO: MACK - use the maze checker function for this
    QVector<Tile*> centerTiles;
            centerTiles.push_back(&maze[(width - 1) / 2][(height - 1) / 2]);
    if (width % 2 == 0) {
            centerTiles.push_back(&maze[ width      / 2][(height - 1) / 2]);
        if (height % 2 == 0) {
            centerTiles.push_back(&maze[(width - 1) / 2][ height      / 2]);
            centerTiles.push_back(&maze[ width      / 2][ height      / 2]);
        }
    }
    else if (height % 2 == 0) {
            centerTiles.push_back(&maze[(width - 1) / 2][ height      / 2]);
    }

    // The queue for the BFS
    QQueue<Tile*> discovered;

    // Set the distances of the center tiles and push them to the queue
    for (Tile* tile : centerTiles) {
        tile->setDistance(0); 
        discovered.enqueue(tile);
    }

    // Now do a BFS
    while (!discovered.empty()){
        Tile* tile = discovered.dequeue();
        for (Direction direction : DIRECTIONS) {
            if (!tile->isWall(direction)) {
                Tile* neighbor = getNeighbor(tile->getX(), tile->getY(), direction);
                if (neighbor != nullptr && neighbor->getDistance() == -1) {
                    neighbor->setDistance(tile->getDistance() + 1);
                    discovered.enqueue(neighbor);
                }
            }
        }
    }

    return maze;
}

} // namespace sim
