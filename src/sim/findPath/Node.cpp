#include "Node.h"

namespace sim{

Node::Node() : m_x(0), m_y(0), m_distance(0), m_explored(false), m_neighbors()
{ }

Node::~Node()
{ }

int Node::getX(){
    return m_x;
}

int Node::getY(){
    return m_y;
}

int Node::getDistance(){
    return m_distance;
}

bool Node::getExplored(){
    return m_explored;
}

std::vector<Node*> Node::getNeighbors(){
    return m_neighbors;
}

void Node::setPos(int x, int y){
    m_x = x;
    m_y = y;
}

void Node::setDistance(int distance){
    m_distance = distance;
}

void Node::setExplored(bool explored){
    m_explored = explored;
}

void Node::addNeighbor(Node* node){
    m_neighbors.push_back(node);
}

} // namespace sim
