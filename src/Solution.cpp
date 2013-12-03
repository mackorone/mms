#include "Solution.h"

#include "MouseInterface.h"

Solution::Solution(MouseInterface* mouse) : m_mouse(mouse)
{ }

Solution::~Solution()
{ }

void Solution::solve(){

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



        randomizedWallFollow();



    // ------------------------------------------------------------------------ //
    // ------------------ Put maze solution above this line ------------------- //
    // ------------------------------------------------------------------------ //
}

void Solution::rightWallStep(){
    if (!m_mouse->wallRight()){
        m_mouse->turnRight();
    }
    while (m_mouse->wallFront()){
        m_mouse->turnLeft();
    }
    m_mouse->moveForward();
}

void Solution::leftWallStep(){
    if (!m_mouse->wallLeft()){
        m_mouse->turnLeft();
    }
    while (m_mouse->wallFront()){
        m_mouse->turnRight();
    }
    m_mouse->moveForward();
}

void Solution::rightWallFollow(){
    while (true){
        rightWallStep();
    }
}

void Solution::leftWallFollow(){
    while (true){
        leftWallStep();
    }
}

void Solution::randomizedWallFollow(){
    while (true){
        if (rand() % 2 == 0){
            rightWallStep();
        }
        else{
            leftWallStep();
        }
    }
}
