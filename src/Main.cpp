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

/*
int main(int argc, char* argv[]) {

    // Initialize local objects
    sim::Maze maze;
    sim::MazeGraphic mazeGraphic(&maze);
    sim::Mouse mouse(&maze);
    sim::MouseGraphic mouseGraphic(&mouse);
    sim::World world(&maze, &mouse);
    sim::MouseInterface mouseInterface(&maze, &mouse, &mazeGraphic);

    // Assign global variables
    g_world = &world;
    g_mazeGraphic = &mazeGraphic;
    g_mouseGraphic = &mouseGraphic;
    g_mouseInterface = &mouseInterface;

    // Initialize the state object (to avoid a race condition)
    sim::S();

    // GLUT Initialization
    glutInit(&argc, argv);
    int pixelsPerTile = (sim::P()->wallLength() + sim::P()->wallWidth()) * sim::P()->pixelsPerMeter();
    glutInitWindowSize(maze.getWidth() * pixelsPerTile, maze.getHeight() * pixelsPerTile);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Micromouse Simulator");
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
*/

/* ADD GLOBAL VARIABLES HERE LATER */
GLuint program;
GLint attribute_coord2d;
 
int init_resources(void) {

    GLint compile_ok = GL_FALSE;
    GLint link_ok = GL_FALSE;
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    std::string str = 
        "#version 120\n"
        "attribute vec2 coord2d;                  "
        "void main(void) {                        "
        "  gl_Position = vec4(coord2d, 0.0, 1.0); " // TODO: Use a uniform to set the properties
        "}";

    const char *vs_source = str.c_str();
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if (0 == compile_ok) {
        fprintf(stderr, "Error in vertex shader\n");
        return 0;
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        fprintf(stderr, "glLinkProgram:");
        return 0;
    }

    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord2d == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }

    return 1;
}
 
void onDisplay() {

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);
    GLfloat triangle_vertices[] = {
         0.0,  0.8,
        -0.8, -0.8,
         0.8, -0.8,
         0.6,  0.6,
    };

    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
        attribute_coord2d, // attribute
        2,                 // number of elements per vertex, here (x,y)
        GL_FLOAT,          // the type of each element
        GL_FALSE,          // take our values as-is
        0,                 // no extra data between each position
        triangle_vertices  // pointer to the C array
    );

    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_POLYGON, 0, 4);
    glDisableVertexAttribArray(attribute_coord2d);

    /* Display the result */
    glutSwapBuffers();
}
 
void free_resources() {
    glDeleteProgram(program);
}
 
int main(int argc, char* argv[]) {

    /* Glut-related initialising functions */
    glutInit(&argc, argv);
    //glutInitContextVersion(2,0); // TODO: Causes issues
    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("My First Triangle");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc(keyInput);

    /* Extension wrangler initialising */
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return EXIT_FAILURE;
    }

      if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
        return 1;
      }


    if (init_resources()) {
        glutDisplayFunc(onDisplay);
        glutMainLoop();
    }

    free_resources();
    return EXIT_SUCCESS;
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
    glutPostRedisplay();
}

void solve() {

    // First, check to ensure that the algorithm is valid
    std::map<std::string, IAlgorithm*> algos = AlgoHub().getAlgorithms();
    if (algos.find(sim::P()->algorithm()) == algos.end()) {
        sim::SimUtilities::print("Error: The algorithm \"" + sim::P()->algorithm() + "\" is not a valid algorithm.");
        sim::SimUtilities::quit();
    }

    // Then, execute the algorithm
    algos.at(sim::P()->algorithm())->solve(g_mouseInterface);
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
