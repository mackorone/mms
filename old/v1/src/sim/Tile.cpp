#include "Tile.h"

#include "Constants.h"
#include "Parameters.h"

namespace sim{

Tile::Tile() : m_x(0), m_y(0), m_distance(MAX_DISTANCE), m_explored(false),
               m_passes(0), m_posp(false), m_neighbors() {
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

int Tile::getDistance(){
    return m_distance;
}

int Tile::getPasses(){
    return m_passes;
}

bool Tile::getExplored(){
    return m_explored;
}

bool Tile::getPosp(){
    return m_posp;
}

bool Tile::isWall(int direction){
    return m_walls[direction];
}

std::vector<Tile*> Tile::getNeighbors(){
    return m_neighbors;
}

void Tile::setPos(int x, int y){
    m_x = x;
    m_y = y;
}

void Tile::setDistance(int distance){
    m_distance = distance;
}

void Tile::incrementPasses(){
    m_passes++;
}

void Tile::resetPasses(){
    m_passes = 0;
}

void Tile::setExplored(bool explored){
    m_explored = explored;
}

void Tile::setPosp(bool posp){
    m_posp = posp;
}

void Tile::setWall(int wall, bool exists){
    m_walls[wall] = exists;
}

void Tile::addNeighbor(Tile* neighbor){
    m_neighbors.push_back(neighbor);
}

void Tile::resetNeighbors(){
    m_neighbors.clear();
}

} // namespace sim
