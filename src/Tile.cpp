#include "Tile.h"

#include "Constants.h"

Tile::Tile() : m_x(0), m_y(0) {
    m_walls[0] = false;
    m_walls[1] = false;
    m_walls[2] = false;
    m_walls[3] = false;
}

Tile::~Tile()
{ }

int Tile::getX(){
    return m_x;
}

int Tile::getY(){
    return m_y;
}

void Tile::setPos(int x, int y){
    m_x = x;
    m_y = y;
}

bool Tile::isWall(int direction){
    return m_walls[direction%4];
}

/*void Tile::setWalls(bool* walls){ // TODO
    m_walls[0] = walls[0];
    m_walls[1] = walls[1];
    m_walls[2] = walls[2];
    m_walls[3] = walls[3];
}*/

void Tile::setWall(int wall, bool exists){
    m_walls[wall] = exists;
}
