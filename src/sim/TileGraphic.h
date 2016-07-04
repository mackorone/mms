#pragma once

#include <QMap>

#include "BufferInterface.h"
#include "Color.h"
#include "Tile.h"

namespace sim {

class TileGraphic {

public:

    TileGraphic();
    TileGraphic(const Tile* tile, BufferInterface* bufferInterface);

    bool wallDeclared(Direction direction) const;

    void setColor(const Color color);
    void declareWall(Direction direction, bool isWall);
    void undeclareWall(Direction direction);
    void setFogginess(bool foggy);
    void setText(const QVector<std::string>& rowsOfText);

    void draw() const;
    void updateColor() const;
    void updateWalls() const;
    void updateFog() const;
    void updateText() const;
    
private:
    const Tile* m_tile;
    BufferInterface* m_bufferInterface;

    Color m_color;
    QMap<Direction, bool> m_declaredWalls;
    bool m_foggy;
    QVector<std::string> m_rowsOfText;

    void updateWall(Direction direction) const;
    std::pair<Color, float> deduceWallColorAndAlpha(Direction direction) const;
};

} // namespace sim
