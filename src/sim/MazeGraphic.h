#pragma once

#include <QVector>

#include "BufferInterface.h"
#include "Color.h"
#include "Maze.h"
#include "TileGraphic.h"

namespace mms {

class MazeGraphic {

public:
    MazeGraphic(const Maze* maze, BufferInterface* bufferInterface);

    void setTileColor(int x, int y, Color color);
    void declareWall(int x, int y, Direction direction, bool isWall);
    void undeclareWall(int x, int y, Direction direction);
    void setTileFogginess(int x, int y, bool foggy);
    void setTileText(int x, int y, const QString& text);

    // TODO: MACK - rename these
    void drawPolygons() const;
    void drawTextures();

    void updateColor() const;
    void updateWalls() const;
    void updateFog() const;
    void updateText() const;

private:
    QVector<QVector<TileGraphic>> m_tileGraphics;

    int getWidth() const;
    int getHeight() const;
    bool withinMaze(int x, int y) const;

};

} // namespace mms
