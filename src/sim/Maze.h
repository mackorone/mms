#pragma once

#include <QByteArray>
#include <QVector>

#include "BasicMaze.h"
#include "Direction.h"
#include "Tile.h"

namespace mms {

class Maze {

public:

    static Maze* fromFile(const QString& path);
    static Maze* fromAlgo(const QByteArray& bytes);
    
    int getWidth() const;
    int getHeight() const;
    bool withinMaze(int x, int y) const;
    const Tile* getTile(int x, int y) const;

    int getMaximumDistance() const;
    bool isValidMaze() const;
    bool isOfficialMaze() const;
    bool isCenterTile(int x, int y) const;

private:

    // Private constructor forces clients to construct
    // a maze using one of the public static methods
    explicit Maze(BasicMaze basicMaze);

    // Vector to hold all of the tiles
    QVector<QVector<Tile>> m_maze;

    // Cache results to these functions
    bool m_isValidMaze;
    bool m_isOfficialMaze;

    // Initializes all of the tiles of the basic maze
    static QVector<QVector<Tile>> initializeFromBasicMaze(const BasicMaze& basicMaze);

    // Basic maze geometric transformations
    static BasicMaze mirrorAcrossVertical(const BasicMaze& basicMaze);
    static BasicMaze rotateCounterClockwise(const BasicMaze& basicMaze);

    // (Re)set the distance values for the tiles in maze that are reachable from the center
    static QVector<QVector<Tile>> setTileDistances(QVector<QVector<Tile>> maze);
};

} // namespace mms
