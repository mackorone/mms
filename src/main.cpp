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

// Declare simulation objects
Maze maze(MAZE_FILE);
Mouse mouse(&maze);
MouseInterface mouseInterface(&mouse);
Solution solution(&mouseInterface);

// Declare graphics objects
MazeGraphic mazeGraphic(&maze);
MouseGraphic mouseGraphic(&mouse);

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

int main(int argc, char* argv[]){


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
