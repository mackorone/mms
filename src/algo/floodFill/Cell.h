#ifndef CELL_H_
#define CELL_H_

/*
 * A class used in the FloodFill algorithm whose purpose is twofold. First, it
 * is used by the explore function of the algorithm to traverse the entirety of
 * the maze. Simultaneously, it is used to keep track of the distance values
 * from the center of the maze. From this, it is used to retrace through the maze
 * to find the shortest path.
 */
class Cell{

public:
    Cell();
    ~Cell();
    
    int getX();
    int getY();
    int getDistance();
    bool getExplored();
    bool isWall(int direction);
    Cell* getPrev();

    void setX(int x);
    void setY(int y);
    void setDistance(int distance);
    void setExplored(bool explored);
    void setWall(int direction, bool isWall);
    void setPrev(Cell* prev);

private:
    int m_x; // x-location of the cell
    int m_y; // y-location of the cell
    int m_distance; // Distance from the goal
    bool m_explored; // Whether or not the Cell was traversed by the mouse
    bool m_walls[4]; // North, East, South, West
    Cell* m_prev; // The previous node in the path by which the Cell was discovered

};

#endif // CELL_H_
