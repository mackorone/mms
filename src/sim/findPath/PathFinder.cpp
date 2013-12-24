#include "PathFinder.h"

#include <iostream>
#include <queue>
#include "Graph.h"

namespace sim{

PathFinder::PathFinder(Maze* maze) : m_graph(maze), m_maze(maze)
{ }

PathFinder::~PathFinder()
{ }

void PathFinder::solveShortestPath(){
    std::vector<Node*> sp = findPathToCenter();
    for (int i = 0; i < sp.size(); i++){
        m_maze->getTile(sp.at(i)->getX(), sp.at(i)->getY())->setPosp(true);
    }
}

std::vector<Node*> PathFinder::findPath(int x1, int y1, int x2, int y2){
    setDistancesFrom(x1, x2);
    return backtrace(m_graph.getNode(x2, y2));
}

std::vector<Node*> PathFinder::findPathToCenter(){
    setDistancesFrom(0, 0);
    return backtrace(getClosestCenterNode());
}

std::vector<Node*> PathFinder::backtrace(Node* end){

    // The vector to hold the solution nodes
    std::vector<Node*> path;

    // Start at the ending node and simply trace back through the values
    std::queue<Node*> queue;
    queue.push(end);
    path.push_back(end);
    while (!queue.empty()){
        Node* node = queue.front();
        queue.pop(); // Removes the element
        std::vector<Node*> neighbors = node->getNeighbors();
        for (int i = 0; i < neighbors.size(); i++){
            if (neighbors.at(i)->getDistance() == node->getDistance() - 1){
                path.push_back(neighbors.at(i));
                queue.push(neighbors.at(i));
            }
        }
    }

    return path;
}

void PathFinder::printDistances(){
    std::cout << std::endl;
    for (int y = m_graph.getHeight()-1; y >= 0; y--){
        for (int x = 0; x < m_graph.getWidth(); x++){
            if (m_graph.getNode(x, y)->getDistance() < 100){
                std::cout << " ";
                if (m_graph.getNode(x, y)->getDistance() < 10){
                    std::cout << " ";
                }
            }
            std::cout << m_graph.getNode(x, y)->getDistance() << " ";
        }
        std::cout << std::endl;
    }
}

void PathFinder::setDistancesFrom(int x, int y){
    
    // Ensure that the nodes are fresh every time this function is called
    m_graph.resetNodes();

    // Queue for BFS
    std::queue<Node*> queue;
    queue.push(m_graph.getNode(x, y));
    m_graph.getNode(x, y)->setDistance(0);
    m_graph.getNode(x, y)->setExplored(true);

    while (!queue.empty()){
        Node* node = queue.front();
        queue.pop(); // Removes the element
        std::vector<Node*> neighbors = node->getNeighbors();
        for (int i = 0; i < neighbors.size(); i++){
            if (!neighbors.at(i)->getExplored()){
                neighbors.at(i)->setDistance(node->getDistance() + 1); 
                queue.push(neighbors.at(i));
                neighbors.at(i)->setExplored(true);
            }
        }
    }
}

Node* PathFinder::getClosestCenterNode(){

    if (m_graph.getWidth() > 0 && m_graph.getHeight() > 0){
    
        int midWidth = m_graph.getWidth()/2;
        int midHeight = m_graph.getHeight()/2;
        Node* closest = m_graph.getNode(midWidth, midHeight);

        if (m_graph.getWidth() % 2 == 0){
            if (m_graph.getNode(midWidth-1, midHeight)->getDistance() < closest->getDistance()){
                closest = m_graph.getNode(midWidth-1, midHeight);
            }
        }
        if (m_graph.getHeight() % 2 == 0){
            if (m_graph.getNode(midWidth, midHeight-1)->getDistance() < closest->getDistance()){
                closest = m_graph.getNode(midWidth, midHeight-1);
            }
        }
        if (m_graph.getWidth() % 2 == 0 && m_graph.getHeight() % 2 == 0){
            if (m_graph.getNode(midWidth-1, midHeight-1)->getDistance() < closest->getDistance()){
                closest = m_graph.getNode(midWidth-1, midHeight-1);
            }
        }
        
        return closest;
    }

    // If either the width or the height is zero return NULL
    return NULL;
}

} // namespace sim
