#pragma once

#include <glut.h>

#include "Tile.h"

namespace sim {

class TileGraphic {

public:

    // TODO: TriangleGraphicCount

    TileGraphic(const Tile* tile);

    void draw() const;
    void setColor(const GLfloat* color);
    void setText(const std::string& text);
    void setFogginess(bool foggy);
    void declareWall(Direction direction, bool isWall);
    void undeclareWall(Direction direction);
    
    bool wallDeclared(Direction direction) const;

private:
    const Tile* m_tile;
    const GLfloat* m_color;
    std::string m_text;
    bool m_foggy;
    std::map<Direction, bool> m_declaredWalls;
};

} // namespace sim
