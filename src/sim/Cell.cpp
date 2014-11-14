#include "Cell.h"

namespace gen{

Cell::Cell() : m_x(0), m_y(0), m_distance(0), m_explored(false) {
    m_walls[0] = false;
    m_walls[1] = false;
    m_walls[2] = false;
    m_walls[3] = false;
}

Cell::~Cell()
{ }

int Cell::getX(){
    return m_x;
}

int Cell::getY(){
    return m_y;
}

int Cell::getDistance(){
    return m_distance;
}

bool Cell::getExplored(){
    return m_explored;
}

void Cell::setDistance(int distance){
    m_distance = distance;
}

void Cell::setExplored(bool explored){
    m_explored = explored;
}

void Cell::setWall(int wall, bool exists){
    m_walls[wall] = exists;
}

} // namespace sim
