#include "Model.h"

namespace mms {

Model::Model() {
    m_maze = new Maze(); 
    m_mouse = new Mouse(m_maze); 
    m_world = new World(m_maze, m_mouse);
}

const Maze* Model::getMaze() const {
    return m_maze;
}

const Mouse* Model::getMouse() const {
    return m_mouse;
}

Mouse* Model::getMouse() {
    return m_mouse;
}

World* Model::getWorld() {
    return m_world;
}

} // namespace mms
