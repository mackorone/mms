#include <GL/freeglut.h>
#include <iostream>
#include <thread>

#include "sim/Constants.h"
#include "sim/Maze.h"
#include "sim/MazeFileUtilities.h"
#include "sim/MazeGraphic.h"
#include "sim/Mouse.h"
#include "sim/MouseGraphic.h"
#include "sim/MouseInterface.h"
#include "sim/Parameters.h"
#include "sim/Tile.h"
#include "sim/Sleep.h"
#include "algo/Solver.h"

// Test Comment

// Function declarations
void draw();
void solve();
void keyInput(unsigned char key, int x, int y);

// Global object variable declarations
Solver* g_solver;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;

// Global primitive variable declarations
static /*non-const*/ bool PAUSED = false; // Initially set to false  
static /*non-const*/ int SLEEP_TIME = 150; // ms between simulation steps
static /*non-const*/ bool UNDO_REQUESTED = false; // Whether or not an undo was requested
static /*non-const*/ bool RESET_REQUESTED = false; // Whether or not a reset was requested

int main(int argc, char* argv[]){
    
    // Ensure that the size parameters are valid
    if (sim::MAZE_WIDTH < 1 || sim::MAZE_HEIGHT < 1){
        std::cout << "Impossible maze size - check \"src/sim/Parameters.h\"" << std::endl;
        return 0;
    }

    // Initialize local simulation objects
    sim::Maze maze(sim::MAZE_WIDTH, sim::MAZE_HEIGHT, sim::getMazeFileDirPath(argv[0]), sim::MAZE_FILE);
    sim::Mouse mouse(&maze);
    sim::MouseInterface mouseInterface(&mouse, &SLEEP_TIME, &PAUSED, &UNDO_REQUESTED, &RESET_REQUESTED);
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
    glutInitWindowSize(sim::WINDOW_WIDTH, sim::WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(sim::MAZE_FILE.c_str());
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
    glFlush();
    sim::sleep(sim::SLEEP_TIME_MIN); // Reduces CPU usage
    glutPostRedisplay();
}

void solve(){
    sim::sleep(250); // Wait for 0.25 seconds for GLUT to intialize
    g_solver->solve();
}

void keyInput(unsigned char key, int x, int y){
    if (key == 32){ // Space bar
        PAUSED = !PAUSED;
    }
    else if (key == 'f' || key == 'F'){ // Faster
        SLEEP_TIME /= 1.15;
        if (SLEEP_TIME < sim::SLEEP_TIME_MIN){
            SLEEP_TIME = sim::SLEEP_TIME_MIN;
        }
    }
    else if (key == 's' || key == 'S'){ // Slower
        SLEEP_TIME *= 1.2;
        if (SLEEP_TIME > sim::SLEEP_TIME_MAX){
            SLEEP_TIME = sim::SLEEP_TIME_MAX;
        }
    }
    else if (key == 'u' || key == 'U'){
        // Undo request - reset the position mouse but retains memory
        UNDO_REQUESTED = true;
    }
    else if (key == 'r' || key == 'R'){
        // Reset requested - reset the position mouse and don't retain memory
        RESET_REQUESTED = true;
    }
    else if (key == 'q' || key == 'Q'){ // Quit
        exit(0);
    }
}
