#ifndef SIM_GRAPH_H_
#define SIM_GRAPH_H_

#include <vector>
#include "../Maze.h"
#include "Node.h"

namespace sim{

class Graph{

public:
    Graph(Maze* maze);
    ~Graph();

    int getWidth();
    int getHeight();
    Node* getNode(int x, int y);
    void resetNodes();
    
private:
    // Table of nodes corresponding to tiles
    std::vector<std::vector<Node>> m_nodes;
};

} // namespace sim

#endif // SIM_GRAPH_H_
