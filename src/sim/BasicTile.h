#pragma once

namespace sim {

struct BasicTile {
    int xPosition; 
    int yPosition; 
    bool isWallNorth; 
    bool isWallEast; 
    bool isWallSouth; 
    bool isWallWest; 
};

} // namespace sim
