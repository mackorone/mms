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
void reshape(int w, int h);
void keyInput(unsigned char key, int x, int y);

// Global object variable declarations
sim::World* g_world;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;
sim::MouseInterface* g_mouseInterface;

// TODO: Other globals here for interfacing with freeglut
//GLuint g_mazeGraphicID;
//GLuint g_mouseGraphicID;
GLuint attribute_coordinate;
GLuint attribute_color;
GLuint vertex_buffer_object;
GLuint vertex_array_object;
void onDisplay();

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
    //glShadeModel(GL_FLAT); // TODO: Good! None of OpenGL interpolates color of pixels between vertices automatically
    //glProvokingVertex(GL_FIRST_VERTEX_CONVENTION); // Use the color of the first vertex for every polygon - 3.2 or higher

    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyInput);
    //glutDisplayFunc(draw); // TODO
    //glutIdleFunc(draw); // TODO


#if(1)
// TODO

    glutDisplayFunc(onDisplay); // TODO

    // GLEW Initialization
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        sim::SimUtilities::print("Error: Unable to initialize GLEW.");
        sim::SimUtilities::quit();
    }

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
    attribute_coordinate = glGetAttribLocation(program, "coordinate");
    if (attribute_coordinate == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", "coordinate");
        return 0;
    }

    // Coordinate attribute
    attribute_color = glGetAttribLocation(program, "color");
    if (attribute_coordinate == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", "color");
        return 0;
    }

    // TODO: Look into wireframe: https://www.youtube.com/watch?v=lRUaQ_Hcno8

    // TODO: Vertex array object (to remember some state)
    //glGenVertexArrays(1, &vertex_array_object);
    //glBindVertexArray(vertex_array_object);

    // TODO :Make our data
    struct Vertex {
        float x;
        float y;
    };

    struct Poly {
        std::vector<Vertex> vertices;
        std::vector<float> color;
    };
    
    Poly one;
    one.vertices.push_back({0.0, 0.0});
    one.vertices.push_back({1.0, 0.0});
    one.vertices.push_back({0.0, 1.0});
    one.color.push_back(1.0);
    one.color.push_back(1.0);
    one.color.push_back(1.0);
    one.color.push_back(1.0);
    /*
    one.color.push_back(0.0);
    one.color.push_back(0.0);
    one.color.push_back(1.0);
    one.color.push_back(1.0);
    one.color.push_back(0.0);
    one.color.push_back(1.0);
    one.color.push_back(0.0);
    one.color.push_back(1.0);
    */

    Poly two;
    two.vertices.push_back({0.0, 0.0});
    two.vertices.push_back({-1.0, 0.0});
    two.vertices.push_back({0.0, -1.0});
    two.color.push_back(1.0);
    two.color.push_back(1.0);
    two.color.push_back(1.0);
    two.color.push_back(1.0);
    /*
    two.color.push_back(1.0);
    two.color.push_back(0.0);
    two.color.push_back(0.0);
    two.color.push_back(1.0);
    two.color.push_back(1.0);
    two.color.push_back(0.0);
    two.color.push_back(0.0);
    two.color.push_back(1.0);
    */

    std::vector<Poly> ps = {one, two};

    glGenBuffers(1,  &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    // TODO: Do I need to edit the information in the buffer?
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,  0 * sizeof(float), 6 * sizeof(float), &one.vertices.front());
    glBufferSubData(GL_ARRAY_BUFFER,  6 * sizeof(float), 12 * sizeof(float), &two.vertices.front());
    glBufferSubData(GL_ARRAY_BUFFER, 12 * sizeof(float), 4 * sizeof(float), &one.color.front());
    glBufferSubData(GL_ARRAY_BUFFER, 16 * sizeof(float), 4 * sizeof(float), &two.color.front());

    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
        attribute_coordinate, // attribute
        2,                    // number of elements per vertex, here (x,y)
        GL_FLOAT,             // the type of each element
        GL_FALSE,             // take our values as-is
        0,                    // no extra data between each position
        0                     // pointer to the C array (TODO: or the position in the buffer)
    );

    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
        attribute_color, // attribute
        1,               // number of elements per vertex, here (x,y)
        GL_FLOAT,        // the type of each element
        GL_FALSE,        // take our values as-is
        0,  // no extra data between each position
        //(char*) NULL + 24 * sizeof(float)         // pointer to the C array
        (char*) NULL + 12 * sizeof(float)         // offset into the buffer
    ); // TODO: Takes a CPU pointer if a VBO doesn't exist

    //glVertexAttribDivisor(0, 1);
    //glVertexAttribDivisor(attribute_color, 4); // TODO: Causes segfault
    glUseProgram(program);

    glEnableVertexAttribArray(attribute_coordinate);
    glEnableVertexAttribArray(attribute_color);

#endif
// TODO

    // Start the physics loop
    std::thread physicsThread(simulate);
    
    // Start the solving loop
    std::thread solvingThread(solve);
    
    // Start the graphics loop
    glutMainLoop();
}

void onDisplay() {

    glClear(GL_COLOR_BUFFER_BIT);

    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_TRIANGLES, 0, 6);
    /*
    int foo[] = {0, 6};
    int bar[] = {3, 3};
    glMultiDrawArrays(GL_TRIANGLES, foo, bar, 2);
    */

    // TODO: We're always using these attributes so not necessary
    //glDisableVertexAttribArray(attribute_coordinate);
    //glDisableVertexAttribArray(attribute_color);

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
    glutPostRedisplay();

    // TODO: Swap the buffers instead... // TODO: Or both...
    glutSwapBuffers();
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

/*
void simulate() {
    g_world->simulate();
}
*/

void simulate() {
    while (true) {
        sim::SimUtilities::sleep(sim::Seconds(1.0));
     // TODO: Use this to test motion in buffer
    }
}

void reshape(int w, int h) {
    // TODO: Use this to update the window size state
    glViewport(0, 0, w, h);
}

void keyInput(unsigned char key, int x, int y) {

    // TODO: Is the uppercase necessary???

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
        // Toggle maze visibility // TODO: Necessary?
        sim::S()->setMazeVisible(!sim::S()->mazeVisible());
    }
    else if (key == 'm' || key == 'M') {
        // Toggle mouse visibility // TODO: Necessary?
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
