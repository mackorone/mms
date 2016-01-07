#include "Model.h"

namespace sim {

Model::Model() {
    m_maze = new Maze(); 
    m_mouse = new Mouse(m_maze); 
}

Maze* Model::getMaze() {
    return m_maze;
}

Mouse* Model::getMouse() {
    return m_mouse;
}

} // namespace sim
