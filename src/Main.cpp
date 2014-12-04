#include <GL/freeglut.h>
#include <iostream>
#include <thread>

#include "algo/Solver.h"
#include "sim/Constants.h"
#include "sim/Param.h"
#include "sim/Maze.h"
#include "sim/MazeGraphic.h"
#include "sim/Mouse.h"
#include "sim/MouseGraphic.h"
#include "sim/MouseInterface.h"
#include "sim/State.h"
#include "sim/Tile.h"
#include "sim/Utilities.h"

// Function declarations
void draw();
void solve();
void keyInput(unsigned char key, int x, int y);

// Global object variable declarations
Solver* g_solver;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;

int main(int argc, char* argv[]){
    
    // TODO: Read in the parameters from the parameters file, rather than assign them

    // TODO: read input file, and check size, use this as value instead of parameter

    // Ensure that the size parameters are valid
    if (sim::P()->MAZE_WIDTH() < 1 || sim::P()->MAZE_HEIGHT() < 1) { // TODO: This should be gone...
        std::cout << "Impossible maze size - check \"src/sim/Parameters.h\"" << std::endl;
        return 0;
    }

    // Initialize local simulation objects
    // TODO: Make the "maze_files" a parameter
    sim::Maze maze(sim::P()->MAZE_WIDTH(), sim::P()->MAZE_HEIGHT(), sim::getProjectDirectory() + "src/maze_files/", sim::P()->MAZE_FILE());
    sim::Mouse mouse(&maze);
    sim::MouseInterface mouseInterface(&mouse);
    Solver solver(&mouseInterface);

    // Initialize the local graphics objects
    sim::MazeGraphic mazeGraphic(&maze);
    sim::MouseGraphic mouseGraphic(&mouse);

    // Assign global variables
    g_solver = &solver;
    g_mazeGraphic = &mazeGraphic;
    g_mouseGraphic = &mouseGraphic;

    // GLUT Initialization
    glutInit(&argc, argv);
    glutInitWindowSize(sim::P()->WINDOW_WIDTH(), sim::P()->WINDOW_HEIGHT());
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(sim::P()->MAZE_FILE().c_str());
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyInput);

    // Start the solving loop (the name "first" is not important)
    std::thread first(solve);
    
    // Start the graphics loop (which never terminates)
    glutMainLoop();
}

// Draw method for the GLUT environment
void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    g_mazeGraphic->draw();
    g_mouseGraphic->draw();

    // Flushes all draws/updates to the screen
    glFlush();

    // TODO: Sleep stuff
    sim::sleep(sim::P()->SLEEP_TIME_MIN()); // Reduces CPU usage
    // 1/framerate -> sec per frame
    // 1000*1000 -> usec per sec
    //usleep(1000*1000/FRAMERATE);

    // Request to execute the draw function again
    glutPostRedisplay();
}

void solve(){
    sim::sleep(250); // Wait for 0.25 seconds for GLUT to intialize
    g_solver->solve();
}

void keyInput(unsigned char key, int x, int y){
    if (key == 32){ // Space bar
        sim::S()->SET_PAUSED(!sim::S()->PAUSED());
    }
    else if (key == 'f' || key == 'F'){ // Faster
        sim::S()->SET_SLEEP_TIME(sim::S()->SLEEP_TIME() / 1.15);
        if (sim::S()->SLEEP_TIME() < sim::P()->SLEEP_TIME_MIN()){
            sim::S()->SET_SLEEP_TIME(sim::P()->SLEEP_TIME_MIN());
        }
    }
    else if (key == 's' || key == 'S'){ // Slower
        sim::S()->SET_SLEEP_TIME(sim::S()->SLEEP_TIME() * 1.2);
        if (sim::S()->SLEEP_TIME() > sim::P()->SLEEP_TIME_MAX()){
            sim::S()->SET_SLEEP_TIME(sim::P()->SLEEP_TIME_MAX());
        }
    }
    else if (key == 'u' || key == 'U'){
        // Undo request - reset the position mouse but retains memory
        sim::S()->SET_UNDO_REQUESTED(true);
    }
    else if (key == 'r' || key == 'R'){
        // Reset requested - reset the position mouse and don't retain memory
        sim::S()->SET_RESET_REQUESTED(true);
    }
    else if (key == 'q' || key == 'Q'){ // Quit
        exit(0);
    }
}
