#include <GL/freeglut.h>
#include <iostream>
#include <thread>

#include "Constants.h"
#include "Maze.h"
#include "MazeGraphic.h"
#include "Mouse.h"
#include "MouseGraphic.h"
#include "MouseInterface.h"
#include "Parameters.h"
#include "Solution.h"
#include "Tile.h"

// TODO: 1.) Make terminal style output at bottom or right of window
//       2.) Resolve pointers/value objects

// Function declarations
std::string getMazeFilePath(std::string path);
void draw();
void solve();

// Declare simulation objects
Maze maze(getMazeFilePath(MAZE_FILE)); // Will this work?
Mouse mouse(&maze);
MouseInterface mouseInterface(&mouse);
Solution solution(&mouseInterface);

// Declare graphics objects
MazeGraphic mazeGraphic(&maze);
MouseGraphic mouseGraphic(&mouse);

// String that will be the path to a particular maze file
std::string mazeFilePath("");

int main(int argc, char* argv[]){
    
    mazeFilePath = argv[0]; // Initially the binary file path

    // GLUT initialization
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(MAZE_FILE.c_str());
    glClearColor (0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(draw);

    // Start the solving loop
    std::thread first(solve);
    
    // Start the graphics loop
    glutMainLoop();
}


std::string getMazeFilePath(std::string mazeFile){
    // TODO: write this so you can run from any directory
    return mazeFile;
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    mazeGraphic.draw();
    mouseGraphic.draw();
    glFlush();
    glutPostRedisplay();
}

void solve(){
    solution.solve();
}

