#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <unistd.h>

#include "Constants.h"
#include "Maze.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "MouseGraphic.h"
#include "MouseInterface.h"
#include "Parameters.h"
#include "Solution.h"
#include "Tile.h"

// Function declarations
void draw();
void solve();
void keyInput(unsigned char key, int x, int y);
std::string getMazeFilePath(std::string path, std::string mazeFile);

// Global variable declarations
Solution* g_solution;
MazeGraphic* g_mazeGraphic;
MouseGraphic* g_mouseGraphic;

int main(int argc, char* argv[]){
    
    // TODO: read input file, and check size, use this as value instead of parameter

    // Ensure that the size parameters are valid
    if (MAZE_WIDTH < 1 || MAZE_HEIGHT < 1){
        std::cout << "Impossible maze size - check \"Parameters.h\"" << std::endl;
        return 0;
    }

    // Initialize local simulation objects
    Maze maze(MAZE_WIDTH, MAZE_HEIGHT, getMazeFilePath(argv[0], MAZE_FILE));
    Mouse mouse(&maze);
    MouseInterface mouseInterface(&mouse, &SLEEP_TIME, &PAUSED);
    Solution solution(&mouseInterface);
    MazeGraphic mazeGraphic(&maze);
    MouseGraphic mouseGraphic(&mouse);

    // Assign global variables
    g_solution = &solution;
    g_mazeGraphic = &mazeGraphic;
    g_mouseGraphic = &mouseGraphic;

    // Initialization GLUT
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(MAZE_FILE.c_str());
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
    glutPostRedisplay();
}

void solve(){
    usleep(1000*500); // Wait for 0.5 seconds for GLUT to intialize
    g_solution->solve();
}

void keyInput(unsigned char key, int x, int y){
    if (key == 32){ // Space bar
        PAUSED = !PAUSED;
    }
    else if (key == 'f' || key == 'F'){ // Faster
        SLEEP_TIME /= 1.15;
        if (SLEEP_TIME < SLEEP_TIME_MIN){
            SLEEP_TIME = SLEEP_TIME_MIN;
        }
    }
    else if (key == 's' || key == 'S'){ // Slower
        SLEEP_TIME *= 1.2;
        if (SLEEP_TIME > SLEEP_TIME_MAX){
            SLEEP_TIME = SLEEP_TIME_MAX;
        }
    }
}

// Initially, path is a path to the binary file that is executed
std::string getMazeFilePath(std::string path, std::string mazeFile){
    
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
