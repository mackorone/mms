#pragma once

#include "Tile.h"

namespace sim {

class TileGraphic {

public:
    TileGraphic(Tile* tile);
    void draw();

private:
    Tile* m_tile;
};

} // namespace sim
