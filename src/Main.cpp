#include <algorithm>
#include <thread>

#include <glut.h>
#include <Seconds.h>

#include "mouse/MouseAlgorithms.h"
#include "sim/GraphicUtilities.h"
#include "sim/Keys.h"
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
void keyPress(unsigned char key, int x, int y);
void specialKeyPress(int key, int x, int y);
void specialKeyRelease(int key, int x, int y);
void initGraphics(int argc, char* argv[]);

// Global variable declarations
sim::World* g_world;
sim::Mouse* g_mouse;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;
sim::MouseInterface* g_mouseInterface;

// The ID of the transformation matrix, which takes triangle graphic objects in
// the physical coordinate system and transforms them into the OpenGL system.
GLuint g_transformationMatixId;

int main(int argc, char* argv[]) {

    // First, we initialize the state object to:
    // 1) Avoid a race condition
    // 2) Register this thread as the main thread
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
    std::thread physicsThread([](){
        g_world->simulate();
    });
    
    // Start the solving loop
    std::thread solvingThread(solve);

    // Start the graphics loop
    sim::S()->enterMainLoop();
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

    // Enable scissoring so that the maps are only draw in specified locations
    glEnable(GL_SCISSOR_TEST);

    // Render the full map
    std::pair<int, int> fullMapPosition = sim::GraphicUtilities::getFullMapPosition();
    std::pair<int, int> fullMapSize = sim::GraphicUtilities::getFullMapSize();
    glScissor(fullMapPosition.first, fullMapPosition.second, fullMapSize.first, fullMapSize.second);
    glUniformMatrix4fv(g_transformationMatixId, 1, GL_TRUE, &sim::GraphicUtilities::getFullMapTransformationMatrix().front());
    glDrawArrays(GL_TRIANGLES, 0, 3 * sim::GraphicUtilities::TGB.size());

    // Render the zoomed map
    std::pair<int, int> zoomedMapPosition = sim::GraphicUtilities::getZoomedMapPosition();
    std::pair<int, int> zoomedMapSize = sim::GraphicUtilities::getZoomedMapSize();
    glScissor(zoomedMapPosition.first, zoomedMapPosition.second, zoomedMapSize.first, zoomedMapSize.second);
    glUniformMatrix4fv(g_transformationMatixId, 1, GL_TRUE, &sim::GraphicUtilities::getZoomedMapTransformationMatrix(
        g_mouse->getInitialTranslation(), g_mouse->getCurrentTranslation(), g_mouse->getCurrentRotation()).front());
    glDrawArrays(GL_TRIANGLES, 0, 3 * sim::GraphicUtilities::TGB.size());

    // We disable scissoring so that the glClear can take effect
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

    // First, check to ensure that the mouse algorithm is valid
    MouseAlgorithms mouseAlgorithms;
    std::map<std::string, IMouseAlgorithm*> algorithms = mouseAlgorithms.getAlgorithms();
    if (algorithms.find(sim::P()->mouseAlgorithm()) == algorithms.end()) {
        sim::SimUtilities::print("Error: The \"" + sim::P()->mouseAlgorithm() + "\" is not a valid mouse algorithm.");
        sim::SimUtilities::quit();
    }
    IMouseAlgorithm* algorithm = algorithms.at(sim::P()->mouseAlgorithm());

    // TODO: MACK - get rid of the initialized field from the mouse

    // Initialize the mouse with the file provided
    std::string mouseFile = algorithm->mouseFile();
    if (!g_mouse->initialize(mouseFile)) {
        sim::SimUtilities::print("Error: Unable to successfully initialize the mouse from \""
            + algorithm->mouseFile() + "\".");
        sim::SimUtilities::quit();
    }

    // Set the interface type
    sim::InterfaceType interfaceType = algorithm->interfaceType();
    // TODO: MACK - refactor this out
    if (interfaceType == sim::UNDECLARED) {
        sim::SimUtilities::print("Error: You may not declare the mouse interface type to be UNDECLARED.");
        sim::SimUtilities::quit();
    }
    sim::S()->setInterfaceType(algorithm->interfaceType());

    // Wait for everything to stabilize
    sim::SimUtilities::sleep(sim::Seconds(sim::P()->glutInitDuration()));

    // Unfog the beginning tile if necessary
    if (sim::S()->interfaceType() == sim::DISCRETE && sim::P()->discreteInterfaceUnfogTileOnEntry()) {
        g_mazeGraphic->setTileFogginess(0, 0, false);
    }

    // Finally, begin execution of the mouse algorithm
    algorithm->solve(g_mazeGraphic->getWidth(), g_mazeGraphic->getHeight(), g_mouseInterface);
}

void keyPress(unsigned char key, int x, int y) {

    if (key == sim::SPACE) {
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
    else if (key == 'r') {
        // Toggle rotate zoomed map
        sim::S()->setRotateZoomedMap(!sim::S()->rotateZoomedMap());
    }
    else if (key == 'i') {
        // Zoom in
        sim::S()->setZoomedMapScale(sim::S()->zoomedMapScale() * 1.5);
    }
    else if (key == 'o') {
        // Zoom out
        sim::S()->setZoomedMapScale(sim::S()->zoomedMapScale() / 1.5);
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
    else if (key == 'g') {
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
        sim::S()->setInputButtonWasPressed(inputButton, true);
    }
}

void specialKeyPress(int key, int x, int y) {
    if (std::find(sim::ARROW_KEYS.begin(), sim::ARROW_KEYS.end(), key) != sim::ARROW_KEYS.end()) {
        sim::S()->setArrowKeyIsPressed(key, true);
    }
}

void specialKeyRelease(int key, int x, int y) {
    if (std::find(sim::ARROW_KEYS.begin(), sim::ARROW_KEYS.end(), key) != sim::ARROW_KEYS.end()) {
        sim::S()->setArrowKeyIsPressed(key, false);
    }
}

void initGraphics(int argc, char* argv[]) {

    // GLUT Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(sim::P()->initialWindowWidth(), sim::P()->initialWindowHeight());
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Micromouse Simulator");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutKeyboardFunc(keyPress);
    glutSpecialFunc(specialKeyPress);
    glutSpecialUpFunc(specialKeyRelease);
    glPolygonMode(GL_FRONT_AND_BACK, sim::S()->wireframeMode() ? GL_LINE : GL_FILL);

    // When the window changes size, notify the graphic utilities
    glutReshapeFunc([](int width, int height){
        glViewport(0,0, width, height);
        sim::GraphicUtilities::setWindowSize(width, height);
    }); 

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
        "uniform mat4 transformationMatrix;"
        "void main(void) {"
        "    gl_Position = transformationMatrix * vec4(coordinate, 0.0, 1.0);"
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
    g_transformationMatixId = glGetUniformLocation(program, "transformationMatrix");
    glEnableVertexAttribArray(coordinate);
    glEnableVertexAttribArray(color);

    // Specify the information within our buffer
    glVertexAttribPointer(coordinate, 2, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), 0);
    glVertexAttribPointer(color, 4, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (char*) NULL + 2 * sizeof(double));

    // Lastly, initially populate the vertex buffer object with tile information
    g_mazeGraphic->draw();
}
