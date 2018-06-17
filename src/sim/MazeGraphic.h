#pragma once

#include <QVector>

#include "BufferInterface.h"
#include "Color.h"
#include "Maze.h"
#include "TileGraphic.h"

namespace mms {

class MazeGraphic {

public:

    MazeGraphic(
        const Maze* maze,
        BufferInterface* bufferInterface);

    void setTileColor(int x, int y, Color color);
    void declareWall(int x, int y, Direction direction, bool isWall);
    void undeclareWall(int x, int y, Direction direction);
    void setTileText(int x, int y, const QString& text);

    // TODO: MACK - rename these
    // TODO: MACK - why is only one of these const?
    void drawPolygons() const;
    void drawTextures();

private:

    QVector<QVector<TileGraphic>> m_tileGraphics;

    int getWidth() const;
    int getHeight() const;
    bool withinMaze(int x, int y) const;

};

} // namespace mms
