#include "Solution.h"

#include <unistd.h>
#include "MouseInterface.h"

Solution::Solution(MouseInterface* mouse) : m_mouse(mouse)
{ }

Solution::~Solution()
{ }

void Solution::solve(){

    usleep(1000*500); // Wait for 0.5 seconds for GLUT to intialize

    /* Valid function calls:
     * 1) m_mouse->wallFront()
     * 2) m_mouse->wallRight()
     * 3) m_mouse->wallLeft()
     * 4) m_mouse->moveForward()
     * 5) m_mouse->turnRight()
     * 6) m_mouse->turnLeft()
     */

    // ------------------------------------------------------------------------ //
    // ------------------ Put maze solution below this line ------------------- //
    // ------------------------------------------------------------------------ //

        //exampleSolutionForMaze3();
        exampleWallFollow();

    // ------------------------------------------------------------------------ //
    // ------------------ Put maze solution above this line ------------------- //
    // ------------------------------------------------------------------------ //
}

void Solution::exampleSolutionForMaze3(){
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->turnLeft();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->moveForward();
        m_mouse->turnRight();
        m_mouse->turnRight();
        m_mouse->moveForward();
        while (true){
            m_mouse->turnRight();
        }
}

void Solution::exampleWallFollow(){
    while (true){
        if (!m_mouse->wallRight()){
            m_mouse->turnRight();
            m_mouse->moveForward();
        }
        else if (m_mouse->wallFront()){
            m_mouse->turnLeft();
        }
        else{
            m_mouse->moveForward();
        }
    }
}
