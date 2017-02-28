#include "MazeViewMutable.h"

namespace mms {

MazeViewMutable::MazeViewMutable(const Maze* maze) :
    MazeView(maze) {
}

MazeGraphic* MazeViewMutable::getMazeGraphic() {
    return &m_mazeGraphic;
}

void MazeViewMutable::initTileGraphicText(int numRows, int numCols) {
    initText(numRows, numCols);
}

} // namespace mms
