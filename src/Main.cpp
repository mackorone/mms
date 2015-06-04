#include <algorithm>
#include <iostream>
#include <thread>

#include <glut.h>
#include "Seconds.h"

#include "algo/AlgoHub.h"
#include "sim/Param.h"
#include "sim/Maze.h"
#include "sim/MazeGraphic.h"
#include "sim/Mouse.h"
#include "sim/MouseGraphic.h"
#include "sim/MouseInterface.h"
#include "sim/State.h"
#include "sim/SimUtilities.h"
#include "sim/World.h"

#include <iostream> // TODO

// Function declarations
void draw();
void solve();
void simulate();
void keyInput(unsigned char key, int x, int y);

// Global object variable declarations
sim::World* g_world;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;
sim::MouseInterface* g_mouseInterface;

// TODO: Other globals here for interfacing with freeglut
GLint g_mazeGraphicID;
GLint g_mouseGraphicID;
GLint attribute_coordinate;
GLint attribute_color;
void onDisplay();

// The program name
std::string g_program;

int main(int argc, char* argv[]) {

    // Save the program name for restarts
    g_program = std::string(argv[0]);

    // Initialize local objects
    sim::Maze maze;
    sim::MazeGraphic mazeGraphic(&maze);
    sim::Mouse mouse(&maze);
    sim::MouseGraphic mouseGraphic(&mouse);
    sim::World world(&maze, &mouse);
    //sim::MouseInterface mouseInterface(&maze, &mouse, &mazeGraphic);
    g_mouseInterface = new sim::MouseInterface(&maze, &mouse, &mazeGraphic);

    // Assign global variables
    g_world = &world;
    g_mazeGraphic = &mazeGraphic;
    g_mouseGraphic = &mouseGraphic;
    //g_mouseInterface = &mouseInterface;

    // Initialize the state object (to avoid a race condition)
    sim::S();

    // GLUT Initialization
    glutInit(&argc, argv);
    int pixelsPerTile = (sim::P()->wallLength() + sim::P()->wallWidth()) * sim::P()->pixelsPerMeter();
    glutInitWindowSize(maze.getWidth() * pixelsPerTile, maze.getHeight() * pixelsPerTile);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Micromouse Simulator");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //glutDisplayFunc(onDisplay); // TODO
    glutDisplayFunc(draw); // TODO
    glutIdleFunc(draw); // TODO
    glutKeyboardFunc(keyInput);

/*
// TODO

    // GLEW Initialization
    glewInit();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    std::string str = 
        "#version 110\n"
        "attribute vec2 coordinate;"
        "attribute vec3 color;"
        "void main(void) {"
        "   gl_Position = vec4(coordinate, 0.0, 1.0);"
        "   gl_FrontColor[0] = color[0];"
        "   gl_FrontColor[1] = color[1];"
        "   gl_FrontColor[2] = color[2];"
        "}";

    const char *vs_source = str.c_str();
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if (0 == compile_ok) {
        fprintf(stderr, "Error in vertex shader\n");
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glLinkProgram(program);
    GLint link_ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        fprintf(stderr, "glLinkProgram:");
        return 0;
    }

    attribute_coordinate = glGetAttribLocation(program, "coordinate");
    if (attribute_coordinate == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", "coordinate");
        return 0;
    }

    attribute_color = glGetAttribLocation(program, "color");
    if (attribute_color == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", "color");
        return 0;
    }

    glUseProgram(program);

// TODO
*/

    // Start the physics loop
    std::thread physicsThread(simulate);
    
    // Start the solving loop
    std::thread solvingThread(solve);
    
    // Start the graphics loop
    glutMainLoop();
}

//GLuint program;
 
void onDisplay() {

    glEnableVertexAttribArray(attribute_coordinate);
    GLfloat triangle_vertices[] = {
         0.0,  0.8,
        -0.8, -0.8,
         0.8, -0.8,
         0.6,  0.6,
    };

    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
        attribute_coordinate, // attribute
        2,                 // number of elements per vertex, here (x,y)
        GL_FLOAT,          // the type of each element
        GL_FALSE,          // take our values as-is
        0,                 // no extra data between each position
        triangle_vertices  // pointer to the C array
    );

    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_POLYGON, 0, 4);
    glDisableVertexAttribArray(attribute_coordinate);

    /* Display the result */
    glutSwapBuffers();
}
 
 
// -------------------------------------------- TODO

void draw() {

    // In order to ensure we're sleeping the correct amount of time, we time
    // the drawing operation and take it into account when we sleep.
    double start(sim::SimUtilities::getHighResTime());

    // Draw the maze and mouse
    glClear(GL_COLOR_BUFFER_BIT);
    g_mazeGraphic->draw();
    g_mouseGraphic->draw();

    // Flushes all draws/updates to the screen
    glFlush();

    // Get the duration of the drawing operation, in seconds. Note that this duration
    // is simply the total number of real seconds that have passed, which is exactly
    // what we want (since the frame-rate is perceived in real-time and not CPU time).
    double end(sim::SimUtilities::getHighResTime());
    double duration = end - start;

    // Notify the user of a late frame
    if (sim::P()->printLateFrames() && duration > 1.0/sim::P()->frameRate()) {
        sim::SimUtilities::print(std::string("A frame was late by ")
            + std::to_string(duration - 1.0/sim::P()->frameRate())
            + std::string(" seconds, which is ")
            + std::to_string((duration - 1.0/sim::P()->frameRate())/(1.0/sim::P()->frameRate()) * 100)
            + std::string(" percent late."));
    }

    // Sleep the appropriate amount of time, base on the drawing duration
    sim::SimUtilities::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->frameRate() - duration)));

    // Request to execute the draw function again
    //glutPostRedisplay();

    // TODO: Swap the buffers instead...
    glutSwapBuffers();
}

void solve() {

    // First, check to ensure that the algorithm is valid
    std::map<std::string, IAlgorithm*> algos = AlgoHub(&g_mouseInterface).getAlgorithms();
    if (algos.find(sim::P()->algorithm()) == algos.end()) {
        sim::SimUtilities::print("Error: The algorithm \"" + sim::P()->algorithm() + "\" is not a valid algorithm.");
        sim::SimUtilities::quit();
    }

    // Then, execute the algorithm
    try {
        algos.at(sim::P()->algorithm())->solve();
    }
    catch (...) {
        delete g_mouseInterface;
    }

    // TODO
}

void simulate() {
    g_world->simulate();
}

void keyInput(unsigned char key, int x, int y) {
    if (key == 32) { // Space bar
        // Pause the simulation (only in discrete mode)
        sim::S()->setPaused(!sim::S()->paused());
    }
    else if (key == 'f' || key == 'F') {
        // Faster (only in discrete mode)
        sim::S()->setSimSpeed(sim::S()->simSpeed() * 1.5);
    }
    else if (key == 's' || key == 'S') {
        // Slower (only in discrete mode)
        sim::S()->setSimSpeed(sim::S()->simSpeed() / 1.5);
    }
    else if (key == 'a' || key == 'A') {
        // Toggle maze visibility
        sim::S()->setMazeVisible(!sim::S()->mazeVisible());
    }
    else if (key == 'm' || key == 'M') {
        // Toggle mouse visibility
        sim::S()->setMouseVisible(!sim::S()->mouseVisible());
    }
    else if (key == 'p' || key == 'P') {
        // Toggle mouse path visibility
        sim::S()->setMousePathVisible(!sim::S()->mousePathVisible());
    }
    else if (key == 't' || key == 'T') {
        // Toggle wall truth visibility
        sim::S()->setWallTruthVisible(!sim::S()->wallTruthVisible());
    }
    else if (key == 'c' || key == 'C') {
        // Toggle tile colors
        sim::S()->setTileColorsVisible(!sim::S()->tileColorsVisible());
    }
    else if (key == 'x' || key == 'X') {
        // Toggle tile text
        sim::S()->setTileTextVisible(!sim::S()->tileTextVisible());
    }
    else if (key == 'o' || key == 'O') {
        // Toggle tile fog
        sim::S()->setTileFogVisible(!sim::S()->tileFogVisible());
    }
    else if (key == 'r' || key == 'R') {
        // Restart - TODO
        //std::system(std::string("\"" + g_program + "\" &").c_str());
        //sim::SimUtilities::quit();
        g_mouseInterface = NULL;
    }
    else if (key == 'q' || key == 'Q') {
        // Quit
        sim::SimUtilities::quit();
    }
    else if (std::string("0123456789").find(key) != std::string::npos) {
        // Press an input button
        int inputButton = std::string("0123456789").find(key);
        sim::S()->setInputButtonPressed(inputButton, true);
    }
}
