#if(0)
#include "PathFinder.h"

#include <iostream>
#include <queue>
#include "Parameters.h"

namespace sim{

PathFinder::PathFinder(Maze* maze) : m_maze(maze)
{ }

PathFinder::~PathFinder()
{ }

void PathFinder::solveShortestPath(){
    std::vector<Tile*> sp = findPathToCenter();
    for (int i = 0; i < sp.size(); i++){
        m_maze->getTile(sp.at(i)->getX(), sp.at(i)->getY())->setPosp(true);
    }
}

std::vector<Tile*> PathFinder::findPath(int x1, int y1, int x2, int y2){
    setDistancesFrom(x1, x2);
    return backtrace(m_maze->getTile(x2, y2));
}

std::vector<Tile*> PathFinder::findPathToCenter(){
    setDistancesFrom(0, 0);
    return backtrace(getClosestCenterTile());
}

std::vector<Tile*> PathFinder::backtrace(Tile* end){

    // The vector to hold the solution nodes
    std::vector<Tile*> path;

    // Start at the ending node and simply trace back through the values
    std::queue<Tile*> queue;
    queue.push(end);
    path.push_back(end);
    while (!queue.empty()){
        Tile* node = queue.front();
        queue.pop(); // Removes the element
        std::vector<Tile*> neighbors = node->getNeighbors();
        for (int i = 0; i < neighbors.size(); i++){
            if (neighbors.at(i)->getDistance() == node->getDistance() - 1){
                path.push_back(neighbors.at(i));
                queue.push(neighbors.at(i));
            }
        }
    }

    return path;
}

void PathFinder::setDistancesFrom(int x, int y){
    
    // Ensure that the nodes are fresh every time this function is called
    for (int x = 0; x < m_maze->getWidth(); x++){
        for (int y = 0; y < m_maze->getHeight(); y++){
            Tile* tile = m_maze->getTile(x, y);
            tile->setDistance(MAX_DISTANCE);
            tile->setExplored(false);
            tile->setPosp(false);
        }
    }

    // Queue for BFS
    std::queue<Tile*> queue;
    queue.push(m_maze->getTile(x, y));
    m_maze->getTile(x, y)->setDistance(0);
    m_maze->getTile(x, y)->setExplored(true);

    while (!queue.empty()){
        Tile* node = queue.front();
        queue.pop(); // Removes the element
        std::vector<Tile*> neighbors = node->getNeighbors();
        for (int i = 0; i < neighbors.size(); i++){
            if (!neighbors.at(i)->getExplored()){
                neighbors.at(i)->setDistance(node->getDistance() + 1); 
                queue.push(neighbors.at(i));
                neighbors.at(i)->setExplored(true);
            }
        }
    }
}

Tile* PathFinder::getClosestCenterTile(){

    if (m_maze->getWidth() > 0 && m_maze->getHeight() > 0){
    
        int midWidth = m_maze->getWidth()/2;
        int midHeight = m_maze->getHeight()/2;
        Tile* closest = m_maze->getTile(midWidth, midHeight);

        if (m_maze->getWidth() % 2 == 0){
            if (m_maze->getTile(midWidth-1, midHeight)->getDistance() < closest->getDistance()){
                closest = m_maze->getTile(midWidth-1, midHeight);
            }
        }
        if (m_maze->getHeight() % 2 == 0){
            if (m_maze->getTile(midWidth, midHeight-1)->getDistance() < closest->getDistance()){
                closest = m_maze->getTile(midWidth, midHeight-1);
            }
        }
        if (m_maze->getWidth() % 2 == 0 && m_maze->getHeight() % 2 == 0){
            if (m_maze->getTile(midWidth-1, midHeight-1)->getDistance() < closest->getDistance()){
                closest = m_maze->getTile(midWidth-1, midHeight-1);
            }
        }
        
        return closest;
    }

    // If either the width or the height is zero return NULL
    return NULL;
}

} // namespace sim
#endif
