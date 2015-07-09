#include <algorithm>
#include <iostream>
#include <thread>

#include <glut.h>
#include <Milliseconds.h>
#include <Seconds.h>

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

// Function declarations
void draw();
void solve();
void simulate();
void keyInput(unsigned char key, int x, int y);
void initGraphics(int argc, char* argv[]);

// Global object variable declarations
sim::World* g_world;
sim::Mouse* g_mouse;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;
sim::MouseInterface* g_mouseInterface;

GLuint camera; // TODO

int main(int argc, char* argv[]) {

    // First, we initialize the state object (to avoid a race condition)
    sim::S();

    // Initialize local objects
    sim::Maze maze;
    sim::Mouse mouse(&maze);
    sim::World world(&maze, &mouse);
    sim::MazeGraphic mazeGraphic(&maze);
    sim::MouseGraphic mouseGraphic(&mouse);
    sim::MouseInterface mouseInterface(&maze, &mouse, &mazeGraphic);

    // Assign global variables
    g_world = &world;
    g_mouse = &mouse;
    g_mazeGraphic = &mazeGraphic;
    g_mouseGraphic = &mouseGraphic;
    g_mouseInterface = &mouseInterface;

    // Initialize all of the graphics
    initGraphics(argc, argv);

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

    // Clear the vertex buffer object and copy over the CPU buffer
    glBufferData(GL_ARRAY_BUFFER, sim::GraphicUtilities::TGB.size() * sizeof(sim::TriangleGraphic), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sim::GraphicUtilities::TGB.size() * sizeof(sim::TriangleGraphic),
        &sim::GraphicUtilities::TGB.front());

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the full map
    glEnable(GL_SCISSOR_TEST);
    glScissor(sim::P()->fullMapPositionX(), sim::P()->fullMapPositionY(), sim::P()->fullMapWidth(), sim::P()->fullMapHeight());
    glUniformMatrix4fv(camera, 1, GL_TRUE, &sim::GraphicUtilities::getFullMapCameraMatrix().front());
    glDrawArrays(GL_TRIANGLES, 0, 3 * sim::GraphicUtilities::TGB.size());
    glDisable(GL_SCISSOR_TEST);

    // Render the zoomed map
    glEnable(GL_SCISSOR_TEST);
    glScissor(sim::P()->zoomedMapPositionX(), sim::P()->zoomedMapPositionY(), sim::P()->zoomedMapWidth(), sim::P()->zoomedMapHeight());
    glUniformMatrix4fv(camera, 1, GL_TRUE, &sim::GraphicUtilities::getZoomedMapCameraMatrix(
        g_mouse->getCurrentTranslation(), g_mouse->getCurrentRotation()).front());
    glDrawArrays(GL_TRIANGLES, 0, 3 * sim::GraphicUtilities::TGB.size());
    glDisable(GL_SCISSOR_TEST);

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
    else if (key == 'w') {
        // Toggle wireframe mode
        sim::S()->setWireframeMode(!sim::S()->wireframeMode());
        glPolygonMode(GL_FRONT_AND_BACK, sim::S()->wireframeMode() ? GL_LINE : GL_FILL);
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

void initGraphics(int argc, char* argv[]) {

    // GLUT Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(sim::P()->windowWidth(), sim::P()->windowHeight());
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Micromouse Simulator");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutKeyboardFunc(keyInput);
    glPolygonMode(GL_FRONT_AND_BACK, sim::S()->wireframeMode() ? GL_LINE : GL_FILL);

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

    // Generate the vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    std::string str = 
        "#version 110\n"
        "attribute vec2 coordinate;"
        "attribute vec4 color;"
        "uniform mat4 camera;"
        "void main(void) {"
        "    gl_Position = camera * vec4(coordinate, 0.0, 1.0);"
        "    gl_FrontColor = color;"
        "}";
    const char *vs_source = str.c_str();
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);

    // Generate the rendering program
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glLinkProgram(program);
    glUseProgram(program);

    // Retrieve the attribute IDs and enable our attributes
    GLuint coordinate = glGetAttribLocation(program, "coordinate");
    GLuint color = glGetAttribLocation(program, "color");
    camera = glGetUniformLocation(program, "camera");
    glEnableVertexAttribArray(coordinate);
    glEnableVertexAttribArray(color);

    // Specify the information within our buffer
    glVertexAttribPointer(coordinate, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*) NULL + 2 * sizeof(float));

    // Lastly, initially populate the vertex buffer object with tile information
    g_mazeGraphic->draw();
}
