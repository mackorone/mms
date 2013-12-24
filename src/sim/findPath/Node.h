#ifndef SIM_NODE_H_
#define SIM_NODE_H_

#include <vector>

namespace sim{

class Node{

public:
    Node();
    ~Node();

    int getX();
    int getY();
    int getDistance();
    bool getExplored();
    std::vector<Node*> getNeighbors();

    void setPos(int x, int y);
    void setDistance(int distance);
    void setExplored(bool explored);
    void addNeighbor(Node* node);

private:
    int m_x;
    int m_y;
    int m_distance;
    int m_explored;
    std::vector<Node*> m_neighbors;

};

} // namespace sim

#endif // SIM_NODE_H_
