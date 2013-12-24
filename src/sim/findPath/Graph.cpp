#include "Graph.h"

#include "../Constants.h"

namespace sim{

// Initialize the graph with the maze
Graph::Graph(Maze* maze){
    
    // First create all of the nodes
    for (int x = 0; x < maze->getWidth(); x++){
        std::vector<Node> row;
        for (int y = 0; y < maze->getHeight(); y++){
            Node node;
            node.setPos(x, y);
            row.push_back(node);
        }        
        m_nodes.push_back(row);
    }        

    resetNodes(); // Assure that we assign initial values to the nodes

    // Second add the neighbors
    for (int x = 0; x < maze->getWidth(); x++){
        for (int y = 0; y < maze->getHeight(); y++){
            if (!maze->getTile(x, y)->isWall(NORTH)){
                m_nodes.at(x).at(y).addNeighbor(&m_nodes.at(x).at(y+1));
            }
            if (!maze->getTile(x, y)->isWall(EAST)){
                m_nodes.at(x).at(y).addNeighbor(&m_nodes.at(x+1).at(y));
            }
            if (!maze->getTile(x, y)->isWall(SOUTH)){
                m_nodes.at(x).at(y).addNeighbor(&m_nodes.at(x).at(y-1));
            }
            if (!maze->getTile(x, y)->isWall(WEST)){
                m_nodes.at(x).at(y).addNeighbor(&m_nodes.at(x-1).at(y));
            }
        }        
    }        
}

Graph::~Graph()
{ }

int Graph::getWidth(){
    return m_nodes.size();
}

int Graph::getHeight(){
    if (getWidth() > 0){
        return m_nodes.at(0).size();
    }
    return 0;
}

Node* Graph::getNode(int x, int y){
    return &m_nodes.at(x).at(y);
}

void Graph::resetNodes(){
    if (m_nodes.size() > 0){
        for (int x = 0; x < m_nodes.at(0).size(); x++){
            for (int y = 0; y < m_nodes.size(); y++){
                m_nodes.at(x).at(y).setExplored(false);
                m_nodes.at(x).at(y).setDistance(16*16);
            }
        }
    }
}

} // namespace sim
