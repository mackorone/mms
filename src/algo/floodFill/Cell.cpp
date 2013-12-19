#include "Cell.h"

Cell::Cell()
{ }

Cell::~Cell()
{ }

int Cell::distance(){
    return m_distance;
}

void Cell::setDistance(int distance){
    m_distance = distance;
}

bool Cell::isWall(int direction){
    return m_walls[direction];
}

void Cell::setWall(int direction, bool isWall){
    m_walls[direction] = isWall;
}
