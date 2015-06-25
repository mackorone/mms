#pragma once

#include <glut.h>

#include "Tile.h"

namespace sim {

class TileGraphic {

public:

    TileGraphic(const Tile* tile);

    bool wallDeclared(Direction direction) const;

    void setColor(const GLfloat* color);
    void setText(const std::string& text);
    void setFogginess(bool foggy);
    void declareWall(Direction direction, bool isWall);
    void undeclareWall(Direction direction);

    void draw() const;
    void updateColor() const;
    void updateWalls() const;
    void updateFog() const;
    
private:
    const Tile* m_tile;
    const GLfloat* m_color;
    std::string m_text;
    bool m_foggy;
    std::map<Direction, bool> m_declaredWalls;

    void updateWall(Direction direction) const;
    std::pair<const GLfloat*, GLfloat> deduceWallColorAndAlpha(Direction direction) const;
};

} // namespace sim
