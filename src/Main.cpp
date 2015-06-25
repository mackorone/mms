#include <algorithm>
#include <iostream>
#include <thread>

#include <glut.h>
#include "Seconds.h"

#include "algo/AlgoHub.h"
#include "sim/GraphicUtilities.h"
#include "sim/Maze.h"
#include "sim/MazeGraphic.h"
#include "sim/Mouse.h"
#include "sim/MouseGraphic.h"
#include "sim/MouseInterface.h"
#include "sim/Param.h"
#include "sim/State.h"
#include "sim/SimUtilities.h"
#include "sim/TriangleGraphic.h"
#include "sim/World.h"

#include <iostream> // TODO

// Function declarations
void draw();
void solve();
void simulate();
void reshape(int w, int h);
void keyInput(unsigned char key, int x, int y);

// Global object variable declarations
sim::World* g_world;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;
sim::MouseInterface* g_mouseInterface;

int main(int argc, char* argv[]) {

    // Initialize local objects
    sim::Maze maze;
    sim::Mouse mouse(&maze);
    sim::World world(&maze, &mouse);
    sim::MazeGraphic mazeGraphic(&maze);
    sim::MouseGraphic mouseGraphic(&mouse);
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
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    int pixelsPerTile = (sim::P()->wallLength() + sim::P()->wallWidth()) * sim::P()->pixelsPerMeter();
    glutInitWindowSize(maze.getWidth() * pixelsPerTile, maze.getHeight() * pixelsPerTile);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Micromouse Simulator");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyInput);


#if(1)

    // GLEW Initialization
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        sim::SimUtilities::print("Error: Unable to initialize GLEW.");
        sim::SimUtilities::quit();
    }

    // Generate vertex buffer object
    GLuint vertex_buffer_object;
    glGenBuffers(1,  &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    // Vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    std::string str = 
        "#version 110\n"
        "attribute vec2 coordinate;"
        "attribute vec4 color;"
        "void main(void) {"
        "    gl_Position = vec4(coordinate, 0.0, 1.0);"
        "    gl_FrontColor = color;"
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

    /*
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs_source =
        "#version 110\n"
        "void main(void) {"
        "    gl_FragColor[0] = gl_FragCoord.x/640.0;"
        "    gl_FragColor[1] = gl_FragCoord.y/480.0;"
        "    gl_FragColor[2] = 0.5;"
        "    gl_FragColor[3] = 1.0;"
        "}";
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        fprintf(stderr, "Error in fragment shader\n");
        return 0;
    }
    */

    // Program
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    //glAttachShader(program, fs);
    glLinkProgram(program);
    GLint link_ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        fprintf(stderr, "glLinkProgram:");
        return 0;
    }

    // Coordinate attribute
    GLuint coordinate = glGetAttribLocation(program, "coordinate");
    if (coordinate == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", "coordinate");
        return 0;
    }

    // Coordinate attribute
    GLuint color = glGetAttribLocation(program, "color");
    if (coordinate == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", "color");
        return 0;
    }

    // TODO: Vertex array object (to remember some state)
    //glGenVertexArrays(1, &vertex_array_object);
    //glBindVertexArray(vertex_array_object);

    glVertexAttribPointer(coordinate, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*) NULL + 2 * sizeof(float));
    glEnableVertexAttribArray(coordinate);
    glEnableVertexAttribArray(color);
    glUseProgram(program);

    // Populate the vertex buffer object with tile information
    g_mazeGraphic->draw();

#endif

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
    double start(sim::SimUtilities::getHighResTime());

    // Determine the starting index of the mouse
    static const int mouseTrianglesStartingIndex = sim::GraphicUtilities::TGB.size();

    // Make space for mouse updates and copy to the CPU buffer
    sim::GraphicUtilities::TGB.erase(sim::GraphicUtilities::TGB.begin() + mouseTrianglesStartingIndex, sim::GraphicUtilities::TGB.end());

    // Fill the CPU buffer with new mouse triangles
    g_mouseGraphic->draw();

    // Copy the CPU buffer to the vertex buffer object
    // TODO: I should only have to do this once... but only after we know the number of triangles from the mouse
    glBufferData(GL_ARRAY_BUFFER, sim::GraphicUtilities::TGB.size() * sizeof(sim::TriangleGraphic), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sim::GraphicUtilities::TGB.size() * sizeof(sim::TriangleGraphic), &sim::GraphicUtilities::TGB.front());

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Push each element in the vertex buffer object to the vertex shader
    glDrawArrays(GL_TRIANGLES, 0, 3 * sim::GraphicUtilities::TGB.size());

    // Display the result
    glutSwapBuffers();

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
    AlgoHub algoHub;
    std::map<std::string, IAlgorithm*> algos = algoHub.getAlgorithms();
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

void reshape(int w, int h) {
    // TODO: Use this to update the window size state
    // TODO: Is this necessary? It only does needs the window size once?
    glViewport(0, 0, w, h);
}

void keyInput(unsigned char key, int x, int y) {

    if (key == 32) { // Space bar
        // Pause the simulation (only in discrete mode)
        sim::S()->setPaused(!sim::S()->paused());
    }
    else if (key == 'f') {
        // Faster (only in discrete mode)
        sim::S()->setSimSpeed(sim::S()->simSpeed() * 1.5);
    }
    else if (key == 's') {
        // Slower (only in discrete mode)
        sim::S()->setSimSpeed(sim::S()->simSpeed() / 1.5);
    }
    else if (key == 'p') {
        // Toggle mouse path visibility
        sim::S()->setMousePathVisible(!sim::S()->mousePathVisible());
    }
    else if (key == 't') {
        // Toggle wall truth visibility
        sim::S()->setWallTruthVisible(!sim::S()->wallTruthVisible());
        g_mazeGraphic->updateWalls();
    }
    else if (key == 'c') {
        // Toggle tile colors
        sim::S()->setTileColorsVisible(!sim::S()->tileColorsVisible());
        g_mazeGraphic->updateColor();
    }
    else if (key == 'x') {
        // Toggle tile text
        sim::S()->setTileTextVisible(!sim::S()->tileTextVisible());
    }
    else if (key == 'o') {
        // Toggle tile fog
        sim::S()->setTileFogVisible(!sim::S()->tileFogVisible());
        g_mazeGraphic->updateFog();
    }
    else if (key == 'q') {
        // Quit
        sim::SimUtilities::quit();
    }
    else if (std::string("0123456789").find(key) != std::string::npos) {
        // Press an input button
        int inputButton = std::string("0123456789").find(key);
        sim::S()->setInputButtonPressed(inputButton, true);
    }
}
