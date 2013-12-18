#ifndef FLOODFILL_H_
#define FLOODFILL_H_

#include "../IAlgorithm.h"
#include "Cell.h"

// Constants
static const int MAZE_SIZE = 16;
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};


class FloodFill : public IAlgorithm{

public:
    void solve(MouseInterface* mouse);

private:
    MouseInterface* m_mouse;
    Cell m_cells[MAZE_SIZE][MAZE_SIZE];
    int m_x; // X position
    int m_y; // Y position
    int m_d; // Direction

    void printDistances();
    void printWalls();
    void initialize();

    void walls(); // Updates the walls surrounding the robot
    void flood(int x, int y); // Floods the maze corresponding to new walls
    void move(); // Moves the robot into position
    bool inGoal(); // Returns true if the mouse is in the center

    void moveForward(); // Updates x and y and moves mouse
    void turnRight(); // Updates direction and turns mouse
    void turnLeft(); // Updates direction and turns mouse

    Cell* getFrontCell(int x, int y, int d); // Returns the cell in front
    Cell* getLeftCell(int x, int y, int d); // Returns the cell to the right
    Cell* getRightCell(int x, int y, int d); // Returns the cell to the left
    Cell* getRearCell(int x, int y, int d); // Returns the cell in the rear
    bool spaceFront(int x, int y, int d); // Returns true if there's a cell in front
    bool spaceLeft(int x, int y, int d); // Returns true if there's a cell to the left
    bool spaceRight(int x, int y, int d); // Returns true if there's a cell to the right
    int min(int one, int two, int three, int four); // Returns the min of four ints

    // Can be used to set the destination to any cell
    void initializeDestinationTile(int x, int y);
};

#endif // FLOODFILL_H_
