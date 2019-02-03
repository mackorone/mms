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

    void setWall(int x, int y, Direction direction);
    void clearWall(int x, int y, Direction direction);

    void setColor(int x, int y, Color color);
    void clearColor(int x, int y);

    void setText(int x, int y, const QString& text);
    void clearText(int x, int y);

    // TODO: upforgrabs
    // Why is only one of these const?
    void drawPolygons() const;
    void drawTextures();

private:

    QVector<QVector<TileGraphic>> m_tileGraphics;

};

} 
