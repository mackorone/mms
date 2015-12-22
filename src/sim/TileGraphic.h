#pragma once

#include "Color.h"
#include "Tile.h"

namespace sim {

class View; // TODO: MACK

class TileGraphic {

public:

    TileGraphic(const Tile* tile, View* view); // TODO: MACK

    bool wallDeclared(Direction direction) const;

    void setColor(const Color color);
    void declareWall(Direction direction, bool isWall);
    void undeclareWall(Direction direction);
    void setFogginess(bool foggy);
    void setText(const std::vector<std::string>& rowsOfText);

    void draw() const;
    void updateColor() const;
    void updateWalls() const;
    void updateFog() const;
    void updateText() const;
    
private:
    const Tile* m_tile;
    View* m_view; // TODO: MACK
    Color m_color;
    std::map<Direction, bool> m_declaredWalls;
    bool m_foggy;
    std::vector<std::string> m_rowsOfText;

    void updateWall(Direction direction) const;
    std::pair<Color, float> deduceWallColorAndAlpha(Direction direction) const;
};

} // namespace sim
