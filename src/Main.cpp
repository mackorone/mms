#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <unistd.h>

#include "sim/Constants.h"
#include "sim/Maze.h"
#include "sim/MazeGraphic.h"
#include "sim/Mouse.h"
#include "sim/MouseGraphic.h"
#include "sim/MouseInterface.h"
#include "sim/Parameters.h"
#include "sim/Tile.h"
#include "algo/Solver.h"

// Function declarations
void draw();
void solve();
void keyInput(unsigned char key, int x, int y);
std::string getMazeFilePath(std::string path, std::string mazeFile);

// Global variable declarations
Solver* g_solver;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;

int main(int argc, char* argv[]){
    
    std::cout << argv[0] << std::endl;

    // TODO: read input file, and check size, use this as value instead of parameter

    // Ensure that the size parameters are valid
    if (sim::MAZE_WIDTH < 1 || sim::MAZE_HEIGHT < 1){
        std::cout << "Impossible maze size - check \"src/sim/Parameters.h\"" << std::endl;
        return 0;
    }

    // Initialize local simulation objects
    sim::Maze maze(sim::MAZE_WIDTH, sim::MAZE_HEIGHT, getMazeFilePath(argv[0], sim::MAZE_FILE));
    sim::Mouse mouse(&maze);
    sim::MouseInterface mouseInterface(&mouse, &sim::SLEEP_TIME, &sim::PAUSED);
    Solver solver(&mouseInterface);

    // Initialize the local graphics objects
    sim::MazeGraphic mazeGraphic(&maze);
    sim::MouseGraphic mouseGraphic(&mouse);

    // Assign global variables
    g_solver = &solver;
    g_mazeGraphic = &mazeGraphic;
    g_mouseGraphic = &mouseGraphic;

    // Initialization GLUT
    glutInit(&argc, argv);
    glutInitWindowSize(sim::WINDOW_WIDTH, sim::WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(sim::MAZE_FILE.c_str());
    glClearColor (0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyInput);

    // Start the solving loop
    std::thread first(solve);
    
    // Start the graphics loop
    glutMainLoop();
}

// Draw method for the GLUT environment
void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    g_mazeGraphic->draw();
    g_mouseGraphic->draw();
    glFlush();
    usleep(1000*sim::SLEEP_TIME_MIN); // Reduces CPU usage
    glutPostRedisplay();
}

void solve(){
    usleep(1000*500); // Wait for 0.5 seconds for GLUT to intialize
    g_solver->solve();
}

void keyInput(unsigned char key, int x, int y){
    if (key == 32){ // Space bar
        sim::PAUSED = !sim::PAUSED;
    }
    else if (key == 'f' || key == 'F'){ // Faster
        sim::SLEEP_TIME /= 1.15;
        if (sim::SLEEP_TIME < sim::SLEEP_TIME_MIN){
            sim::SLEEP_TIME = sim::SLEEP_TIME_MIN;
        }
    }
    else if (key == 's' || key == 'S'){ // Slower
        sim::SLEEP_TIME *= 1.2;
        if (sim::SLEEP_TIME > sim::SLEEP_TIME_MAX){
            sim::SLEEP_TIME = sim::SLEEP_TIME_MAX;
        }
    }
    else if (key == 'q' || key == 'Q'){ // Quit
        exit(0);
    }
}

// Initially, path is a path to the binary file that is executed
std::string getMazeFilePath(std::string path, std::string mazeFile){
    
    // TODO: Better parser to locates the maze files relative to execution
    // Maybe append ".." to the front or something like that

    // Don't attempt to find a file if no argument is supplied
    if (mazeFile == ""){
        std::cout << "No maze file provided. Generating random maze..." << std::endl;
        return "";
    }

    // Remove bin directory path
    for (int i = 0; i < 2; i++){
        int index = path.find_last_of("/\\");
        path = path.substr(0, index);
    }

    // Append data file path
    path += "/src/mazeFiles/";
    path += mazeFile;

    // Sanity check
    std::fstream file(path.c_str());
    if (!file){
        std::cout << "File \"" << path << "\" not found. Generating random maze..." << std::endl;
        return "";
    }

    return path;
}
