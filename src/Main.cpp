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
#include "sim/SimUtilities.h"
#include "sim/World.h"

// Function declarations
void draw();
void solve();
void simulate();
void keyInput(unsigned char key, int x, int y);

// Global object variable declarations
Solver* g_solver;
sim::World* g_world;
// TODO: Graphic object...
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;

// TODO
sim::Mouse* g_mouse;

int main(int argc, char* argv[]) {

    // Seed the random number generator
    srand(sim::P()->randomSeed());

    // Initialize local simulation objects
    sim::Maze maze;
    sim::Mouse mouse;
    sim::World world(&maze, &mouse);
    sim::MouseInterface mouseInterface(&mouse);
    Solver solver(&mouseInterface);

    // Initialize the local graphics objects
    sim::MazeGraphic mazeGraphic(&maze);
    sim::MouseGraphic mouseGraphic(&mouse);

    // Assign global variables
    g_world = &world;
    g_solver = &solver;
    g_mazeGraphic = &mazeGraphic;
    g_mouseGraphic = &mouseGraphic;

    // TODO
    g_mouse= &mouse;

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

    // Start the physics loop
    std::thread physicsThread(simulate);
    
    // Start the solving loop
    std::thread solvingThread(solve);
    
    // Start the graphics loop
    glutMainLoop();
}

void draw() {

    // In order to ensure we're sleeping the correct amount of time, we time
    // the drawing operation and take it into account when we sleep.
    double start(sim::getHighResTime());

    // Draw the maze and mouse
    glClear(GL_COLOR_BUFFER_BIT);
    g_mazeGraphic->draw();
    g_mouseGraphic->draw();

    // Flushes all draws/updates to the screen
    glFlush();

    // Get the duration of the drawing operation, in seconds. Note that this duration
    // is simply the total number of real seconds that have passed, which is exactly
    // what we want (since the framerate is perceived in real-time and not CPU time).
    double end(sim::getHighResTime());
    double duration = end - start;

    // TODO: Fix these late frames
    if (duration > 1.0/sim::P()->frameRate()) {
        std::cout << "A frame was late by " << duration - 1.0/sim::P()->frameRate() << " seconds" << std::endl;
    }

    // Sleep the appropriate amout of time, base on the drawing duration
    sim::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->frameRate() - duration)));

    // Request to execute the draw function again
    glutPostRedisplay();
}

void solve() {
    sim::sleep(sim::Seconds(sim::P()->glutInitTime()));
    g_solver->solve();
}

void simulate() {
    sim::sleep(sim::Seconds(sim::P()->glutInitTime()));
    g_world->simulate();
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
