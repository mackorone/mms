#pragma once

#include "Tile.h"

namespace sim {

class TileGraphic {

public:
    TileGraphic(const Tile* tile);
    void draw();

private:
    const Tile* m_tile;
};

} // namespace sim
