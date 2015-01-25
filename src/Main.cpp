#include <algorithm>
#include <GL/freeglut.h>
#include <iostream>
#include <thread>

#include "algo/Solver.h"
#include "sim/Param.h"
#include "sim/Maze.h"
#include "sim/MazeGraphic.h"
#include "sim/Mouse.h"
#include "sim/MouseGraphic.h"
#include "sim/MouseInterface.h"
#include "sim/State.h"
#include "sim/units/Seconds.h"
#include "sim/Utilities.h"


// Function declarations
void draw();
void solve();
void keyInput(unsigned char key, int x, int y);

// Global object variable declarations
Solver* g_solver;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;

int main(int argc, char* argv[]) {

    // Initialize local simulation objects
    sim::Maze maze;
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
    glutInitWindowSize(sim::P()->windowWidth(), sim::P()->windowHeight());
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(sim::P()->mazeFile().c_str()); // TODO: mazeFile could be wrong/invalid???
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyInput);

    // Start the solving loop
    std::thread solvingThread(solve);
    
    // Start the graphics loop (which never terminates)
    glutMainLoop();
}

void draw() {

    // In order to ensure we're sleeping the correct amount of time, we time
    // the drawing operation and take it into account when we sleep.
    std::clock_t start = std::clock();

    // Draw the maze and mouse
    glClear(GL_COLOR_BUFFER_BIT);
    g_mazeGraphic->draw();
    g_mouseGraphic->draw();

    // Flushes all draws/updates to the screen
    glFlush();

    // The duration of the drawing operation, inseconds
    float duration = (std::clock() - start) / static_cast<float>(CLOCKS_PER_SEC);

    // Sleep the appropriate amout of time, base on the drawing duration
    sim::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->frameRate() - duration)));

    // Request to execute the draw function again
    glutPostRedisplay();
}

void solve() {
    sim::sleep(sim::Seconds(.25)); // Wait for 0.25 seconds for GLUT to intialize
    g_solver->solve();
}

void keyInput(unsigned char key, int x, int y) {
    if (key == 32) { // Space bar
        sim::S()->setPaused(!sim::S()->paused());
    }
    else if (key == 'f' || key == 'F') { // Faster
        sim::S()->setSimSpeed(sim::S()->simSpeed() / 1.5);
        if (sim::S()->simSpeed() < sim::P()->minSimSpeed()) {
            sim::S()->setSimSpeed(sim::P()->minSimSpeed());
        }
    }
    else if (key == 's' || key == 'S') { // Slower
        sim::S()->setSimSpeed(sim::S()->simSpeed() * 1.5);
        if (sim::S()->simSpeed() > sim::P()->maxSimSpeed()) {
            sim::S()->setSimSpeed(sim::P()->maxSimSpeed());
        }
    }
    else if (key == 'u' || key == 'U') {
        // Undo request - reset the position mouse but retains memory
        sim::S()->setUndoRequested(true);
    }
    else if (key == 'r' || key == 'R') {
        // Reset requested - reset the position mouse and don't retain memory
        sim::S()->setResetRequested(true);
    }
    else if (key == 'q' || key == 'Q') { // Quit
        exit(0);
    }
}
