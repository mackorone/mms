#pragma once

#include "MazeView.h"

namespace mms {

class MazeViewMutable : public MazeView {

public:

    MazeViewMutable(const Maze* maze);
    MazeGraphic* getMazeGraphic();
    void initTileGraphicText(int numRows, int numCols);

};

} // namespace mms
