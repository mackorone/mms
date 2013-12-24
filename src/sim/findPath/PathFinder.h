#ifndef SIM_PATHFINDER_H_
#define SIM_PATHFINDER_H_

#include "Graph.h"

namespace sim{

class PathFinder{

public:
    PathFinder(Maze* maze);
    ~PathFinder();

    void solveShortestPath();

private:
    Graph m_graph;
    Maze* m_maze;

    void printDistances();
    void setDistancesFrom(int x, int y);
    Node* getClosestCenterNode();
    std::vector<Node*> backtrace(Node* end);
    std::vector<Node*> findPathToCenter();
    std::vector<Node*> findPath(int x1, int y1, int x2, int y2);
};

} // namespace sim

#endif // SIM_PATHFINDER_H_
