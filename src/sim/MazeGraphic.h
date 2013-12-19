#ifndef SIM_MAZEGRAPHIC_H_
#define SIM_MAZEGRAPHIC_H_

#include <vector>
#include "Maze.h"
#include "TileGraphic.h"

namespace sim{

class MazeGraphic{

public:
    MazeGraphic(Maze* maze);
    ~MazeGraphic();
    void draw();

private:
    Maze* m_maze;
    std::vector<std::vector<TileGraphic>> m_tileGraphics;
};

} // namespace sim

#endif // SIM_MAZEGRAPHIC_H_
