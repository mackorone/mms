#include "MackAlgo.h"

namespace mack {

void MackAlgo::solve(sim::MouseInterface* mouse) {

    m_mouse = mouse;
    m_mouse->declareInterfaceType(sim::DISCRETE);
    m_mouse->colorTile(0, 0, 'B');
    m_mouse->colorTile(0, 1, 'G');
    m_mouse->colorTile(0, 2, 'A');
    m_mouse->colorTile(0, 3, 'Y');
    m_mouse->colorTile(0, 4, 'R');

    while (true) {
        m_mouse->moveForward();
    }
}

} // namespace mack
