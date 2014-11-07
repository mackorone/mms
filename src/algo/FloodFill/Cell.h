#pragma once

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
    bool getTraversed();
    bool isWall(int direction);
    bool getWallInspected(int direction);
    Cell* getPrev();

    void setX(int x);
    void setY(int y);
    void setDistance(int distance);
    void setExplored(bool explored);
    void setTraversed(bool traversed);
    void setWall(int direction, bool isWall);
    void setWallInspected(int direction, bool inspected);
    void setPrev(Cell* prev);

private:
    int m_x; // x-location of the Cell
    int m_y; // y-location of the Cell
    int m_distance; // Distance of the Cell from the goal
    bool m_explored; // Whether or not all walls of the Cell have been determined
    bool m_traversed; // Whether or not the Cell was traversed by the mouse
    bool m_walls[4]; // Whether or not a wall exists on the North, East, South, and West walls
    bool m_wallsInspected[4]; // Whether or not we've inspected the specific walls of a Cell
    Cell* m_prev; // The previous node in the path by which the Cell was discovered

};
