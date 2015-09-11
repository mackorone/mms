#include <thread>

#include <fontstash/fontstash.h>
#include <glut/glut.h>
#include <tdogl/Bitmap.h>
#include <tdogl/Program.h>
#include <tdogl/Shader.h>
#include <tdogl/Texture.h>

#include "mouse/IMouseAlgorithm.h"
#include "mouse/MouseAlgorithms.h"
#include "sim/GraphicUtilities.h"
#include "sim/InterfaceType.h"
#include "sim/Key.h"
#include "sim/Logging.h"
#include "sim/Maze.h"
#include "sim/MazeGraphic.h"
#include "sim/Mouse.h"
#include "sim/MouseGraphic.h"
#include "sim/MouseInterface.h"
#include "sim/Param.h"
#include "sim/SimUtilities.h"
#include "sim/State.h"
#include "sim/TextDrawer.h"
#include "sim/TriangleGraphic.h"
#include "sim/World.h"
#include "sim/units/Seconds.h"

// Function declarations
void draw();
void keyPress(unsigned char key, int x, int y);
void specialKeyPress(int key, int x, int y);
void specialKeyRelease(int key, int x, int y);

// Initialization functions, declared in the order they should be called
void bootstrap();
void initSimObjects();
void initGraphics(int argc, char* argv[]);
void initMouseAlgo();

// Global variable declarations
sim::Mouse* g_mouse;
sim::MazeGraphic* g_mazeGraphic;
sim::MouseGraphic* g_mouseGraphic;
sim::MouseInterface* g_mouseInterface;
sim::World* g_world;
IMouseAlgorithm* g_algorithm;

// TODO: Hide some state

// TODO: MACK - can't be created before glInit is called, so can't be created statically
sim::TextDrawer* textDrawer = nullptr;
int rows = 2; // TODO: MACK
int cols = 3; // TODO: MACK

// TODO: MACK
tdogl::Program* polygonProgram = nullptr;
GLuint polygonVertexArrayObjectId = 0;
GLuint polygonVertexBufferObjectId = 0;

// TODO: MACK
tdogl::Texture* textureAtlas = nullptr;
tdogl::Program* textureProgram = nullptr;
GLuint textureVertexArrayObjectId = 0;
GLuint textureVertexBufferObjectId = 0;

// loads the vertex shader and fragment shader, and links them to make the global textureProgram
static void LoadShaders() {
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(
        sim::SimUtilities::getProjectDirectory() + "/res/shaders/textureVertexShader.txt", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(
        sim::SimUtilities::getProjectDirectory() + "/res/shaders/textureFragmentShader.txt", GL_FRAGMENT_SHADER));
    textureProgram = new tdogl::Program(shaders);
}

// loads a triangle into the VAO global
static void LoadTriangle() {
    // make and bind the VAO
    glGenVertexArrays(1, &textureVertexArrayObjectId);
    glBindVertexArray(textureVertexArrayObjectId);
    
    // make and bind the VBO
    glGenBuffers(1, &textureVertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, textureVertexBufferObjectId);

    // Put the three triangle vertices (XYZ) and texture coordinates (UV) into the VBO
    GLfloat vertexData[15 * rows * cols];
    for (int i = 0; i < rows; i += 1) {
        for (int j = 0; j < cols; j += 1) {
            vertexData[15*cols*i + 15*j +  0] = (j * 1.0 / float(cols)) * 2 - 1;
            vertexData[15*cols*i + 15*j +  1] = (i * 1.0 / float(rows)) * 2 - 1;
            vertexData[15*cols*i + 15*j +  2] = 0.0; // Z
            vertexData[15*cols*i + 15*j +  3] = 0.5; // U
            vertexData[15*cols*i + 15*j +  4] = 1.0; // V
            vertexData[15*cols*i + 15*j +  5] = (j * 1.0 / float(cols) + 0.5 / float(cols)) * 2 - 1;
            vertexData[15*cols*i + 15*j +  6] = ((i + 1) * 1.0 / float(rows)) * 2 - 1;
            vertexData[15*cols*i + 15*j +  7] = 0.0; // Z
            vertexData[15*cols*i + 15*j +  8] = 0.0; // U
            vertexData[15*cols*i + 15*j +  9] = 0.0; // V
            vertexData[15*cols*i + 15*j + 10] = ((j + 1) * 1.0 / float(cols)) * 2 - 1;
            vertexData[15*cols*i + 15*j + 11] = (i * 1.0 / float(rows)) * 2 - 1;
            vertexData[15*cols*i + 15*j + 12] = 0.0; // Z
            vertexData[15*cols*i + 15*j + 13] = 1.0; // U
            vertexData[15*cols*i + 15*j + 14] = 0.0; // V
        }
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(textureProgram->attrib("vert"));
    glVertexAttribPointer(textureProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
        
    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(textureProgram->attrib("vertTexCoord"));
    glVertexAttribPointer(textureProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the VAO
    glBindVertexArray(0);
}

// loads the file "hazard.png" into textureAtlas
static void LoadTexture() {
    tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(sim::SimUtilities::getProjectDirectory() + "/res/textures/hazard.png");
    bmp.flipVertically();
    textureAtlas = new tdogl::Texture(bmp);
}

static void Render() {
    
    // bind the program (the shaders)
    textureProgram->use();
        
    // bind the texture and set the "tex" uniform in the fragment shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlas->object());
    textureProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0

    // bind the VAO (the triangle)
    glBindVertexArray(textureVertexArrayObjectId);
    
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, 15 * rows * cols);
    
    // unbind the VAO, the program and the texture
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    textureProgram->stopUsing();
}

int main(int argc, char* argv[]) {

    // Step 0: Determine the runId, configure logging,
    // and initialize the State and Param objects.
    bootstrap();

    // TODO: MACK
    // Remove any excessive archived runs
    sim::SimUtilities::removeExcessArchivedRuns();

    // Step 1: Initialize the simulation objects
    initSimObjects();

    // Step 2: Initialize the graphics
    initGraphics(argc, argv);

// TODO: MACK -----
    LoadShaders();
    LoadTexture();
    LoadTriangle();
    textDrawer = new sim::TextDrawer("DroidSerif-Regular.ttf", 400.0);
// TODO: MACK -----

    // Step 3: Initialize the mouse algorithm
    initMouseAlgo();

    // Step 4: Start the physics loop
    std::thread physicsThread([](){
        g_world->simulate();
    });

    // Step 5: Start the solving loop
    std::thread solvingThread([](){

        // Wait for the window to appear
        sim::SimUtilities::sleep(sim::Seconds(sim::P()->glutInitDuration()));

        // Unfog the beginning tile if necessary
        if (sim::S()->interfaceType() == sim::InterfaceType::DISCRETE && sim::P()->discreteInterfaceUnfogTileOnEntry()) {
            g_mazeGraphic->setTileFogginess(0, 0, false);
        }

        // Finally, begin execution of the mouse algorithm
        g_algorithm->solve(
            g_mazeGraphic->getWidth(),
            g_mazeGraphic->getHeight(),
            sim::DIRECTION_TO_CHAR.at(sim::STRING_TO_DIRECTION.at(sim::P()->mouseStartingDirection())),
            g_mouseInterface);
    });

    // Step 6: Start the graphics loop
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
    glBindBuffer(GL_ARRAY_BUFFER, polygonVertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, sim::GraphicUtilities::TGB.size() * sizeof(sim::TriangleGraphic), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sim::GraphicUtilities::TGB.size() * sizeof(sim::TriangleGraphic),
        &sim::GraphicUtilities::TGB.front());

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable scissoring so that the maps are only draw in specified locations
    glEnable(GL_SCISSOR_TEST);

    // TODO: MACK ---------------------------
    polygonProgram->use();
    glBindVertexArray(polygonVertexArrayObjectId);
    // TODO: MACK ---------------------------

    // Render the full map
    std::pair<int, int> fullMapPosition = sim::GraphicUtilities::getFullMapPosition();
    std::pair<int, int> fullMapSize = sim::GraphicUtilities::getFullMapSize();
    glScissor(fullMapPosition.first, fullMapPosition.second, fullMapSize.first, fullMapSize.second);
    polygonProgram->setUniformMatrix4("transformationMatrix",
        &sim::GraphicUtilities::getFullMapTransformationMatrix().front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, 0, 3 * sim::GraphicUtilities::TGB.size());

    // Render the zoomed map
    std::pair<int, int> zoomedMapPosition = sim::GraphicUtilities::getZoomedMapPosition();
    std::pair<int, int> zoomedMapSize = sim::GraphicUtilities::getZoomedMapSize();
    glScissor(zoomedMapPosition.first, zoomedMapPosition.second, zoomedMapSize.first, zoomedMapSize.second);
    polygonProgram->setUniformMatrix4("transformationMatrix",
        &sim::GraphicUtilities::getZoomedMapTransformationMatrix(
        g_mouse->getInitialTranslation(), g_mouse->getCurrentTranslation(), g_mouse->getCurrentRotation()).front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, 0, 3 * sim::GraphicUtilities::TGB.size());

    // TODO: MACK ---------------------------
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    polygonProgram->stopUsing();
    // TODO: MACK ---------------------------

    // We disable scissoring so that the glClear can take effect
    glDisable(GL_SCISSOR_TEST);

#if(1)

    // TODO: MACK - Font-stash drawing
    textDrawer->commenceDrawingTextForFrame();
    auto size = sim::GraphicUtilities::getWindowSize();
    textDrawer->drawText(size.first - 400, size.second - 400, "X");
    textDrawer->concludeDrawingTextForFrame();

// TODO: MACK
    Render();
// TODO: MACK

#endif

    // Display the result
    glutSwapBuffers();

    // Get the duration of the drawing operation, in seconds. Note that this duration
    // is simply the total number of real seconds that have passed, which is exactly
    // what we want (since the frame-rate is perceived in real-time and not CPU time).
    double end(sim::SimUtilities::getHighResTime());
    double duration = end - start;

    // Notify the user of a late frame
    if (duration > 1.0/sim::P()->frameRate()) {
        IF_PRINT_ELSE_LOG(sim::P()->printLateFrames(), WARN,
            "A frame was late by %v seconds, which is %v percent late.",
            duration - 1.0/sim::P()->frameRate(),
            (duration - 1.0/sim::P()->frameRate())/(1.0/sim::P()->frameRate()) * 100);
    }

    // Sleep the appropriate amount of time, base on the drawing duration
    sim::SimUtilities::sleep(sim::Seconds(std::max(0.0, 1.0/sim::P()->frameRate() - duration)));
}

void keyPress(unsigned char key, int x, int y) {

    if (key == 'p') {
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
    else if (key == 'l') {
        // Cycle through the available layouts
        sim::S()->setLayout(sim::LAYOUT_CYCLE.at(sim::S()->layout()));
    }
    else if (key == 'r') {
        // Toggle rotate zoomed map, but only if zoomed map is visible
        if (sim::S()->layout() != sim::Layout::FULL) {
            sim::S()->setRotateZoomedMap(!sim::S()->rotateZoomedMap());
        }
    }
    else if (key == 'i') {
        // Zoom in, but only if zoomed map is visible
        if (sim::S()->layout() != sim::Layout::FULL) {
            sim::S()->setZoomedMapScale(sim::S()->zoomedMapScale() * 1.5);
        }
    }
    else if (key == 'o') {
        // Zoom out, but only if zoomed map is visible
        if (sim::S()->layout() != sim::Layout::FULL) {
            sim::S()->setZoomedMapScale(sim::S()->zoomedMapScale() / 1.5);
        }
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
    if (sim::SimUtilities::vectorContains(sim::ARROW_KEYS, sim::INT_TO_KEY.at(key))) {
        sim::S()->setArrowKeyIsPressed(sim::INT_TO_KEY.at(key), true);
    }
}

void specialKeyRelease(int key, int x, int y) {
    if (sim::SimUtilities::vectorContains(sim::ARROW_KEYS, sim::INT_TO_KEY.at(key))) {
        sim::S()->setArrowKeyIsPressed(sim::INT_TO_KEY.at(key), false);
    }
}

void bootstrap() {

    // First, determine the runId (just datetime, for now)
    std::string runId = sim::SimUtilities::getDateTime();

    // Then we can initiliaze Logging
    sim::Logging::initialize(runId);

    // Initialize the State object in order to:
    // 0) Set the runId
    // 1) Avoid a race condition (between threads)
    // 2) Register this thread as the main thread
    // 3) Initialize the Param object
    sim::S()->setRunId(runId);
}

void initSimObjects() {
    sim::Maze* maze = new sim::Maze();
    g_mouse = new sim::Mouse(maze);
    g_mazeGraphic = new sim::MazeGraphic(maze);
    g_mouseGraphic = new sim::MouseGraphic(g_mouse);
    g_mouseInterface = new sim::MouseInterface(maze, g_mouse, g_mazeGraphic);
    g_world = new sim::World(maze, g_mouse);
}

void initGraphics(int argc, char* argv[]) {

    // GLUT Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(sim::P()->defaultWindowWidth(), sim::P()->defaultWindowHeight());
    sim::GraphicUtilities::setWindowSize(sim::P()->defaultWindowWidth(), sim::P()->defaultWindowHeight());
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
        PRINT(ERROR, "Unable to initialize GLEW.");
        sim::SimUtilities::quit();
    }

    // TODO: these could probably go a bit lower....
    glGenVertexArrays(1, &polygonVertexArrayObjectId); // TODO: MACK
    glBindVertexArray(polygonVertexArrayObjectId); // TODO: MACK

    // Generate vertex buffer object
    ///*GLuint*/ polygonVertexBufferObjectId; // TODO: MACK
    glGenBuffers(1,  &polygonVertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, polygonVertexBufferObjectId);

    // TODO: MACK
    polygonProgram = new tdogl::Program({tdogl::Shader::shaderFromFile(
        sim::SimUtilities::getProjectDirectory() + "/res/shaders/polygonVertexShader.txt", GL_VERTEX_SHADER)});
    glEnableVertexAttribArray(polygonProgram->attrib("coordinate"));
    glVertexAttribPointer(polygonProgram->attrib("coordinate"), 2, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), 0);
    glEnableVertexAttribArray(polygonProgram->attrib("color"));
    glVertexAttribPointer(polygonProgram->attrib("color"), 4, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (char*) NULL + 2 * sizeof(double));

    // unbind the VAO
    glBindVertexArray(0);

    // Lastly, initially populate the vertex buffer object with tile information
    g_mazeGraphic->draw();

    // TODO
    //glBindBuffer(GL_ARRAY_BUFFER, 0); // TODO: MACK
    //glBindVertexArray(0); // TODO: MACK
}

void initMouseAlgo() {

    // First, check to ensure that the mouse algorithm is valid
    if (!MouseAlgorithms::isMouseAlgorithm(sim::P()->mouseAlgorithm())) {
        // TODO: MACK - make a note about the file where it needs to be declared
        PRINT(ERROR, "\"%v\" is not a valid mouse algorithm.",
            sim::P()->mouseAlgorithm());
        sim::SimUtilities::quit();
    }
    g_algorithm = MouseAlgorithms::getMouseAlgorithm(sim::P()->mouseAlgorithm());

    // Initialize the mouse with the file provided
    std::string mouseFile = g_algorithm->mouseFile();
    bool success = g_mouse->initialize(mouseFile);
    if (!success) {
        PRINT(ERROR,
            "Unable to successfully initialize the mouse in the algorithm "
            "\"%v\" from \"%v\".",
            sim::P()->mouseAlgorithm(),
            mouseFile);
        sim::SimUtilities::quit();
    }

    // Initialize the interface type
    if (!sim::SimUtilities::mapContains(sim::STRING_TO_INTERFACE_TYPE, g_algorithm->interfaceType())) {
        PRINT(ERROR,
            "\"%v\" is not a valid interface type. You must declare the "
            "interface type of the mouse algorithm \"%v\" to be either \"%v\" "
            "or \"%v\".",
            g_algorithm->interfaceType(),
            sim::P()->mouseAlgorithm(),
            sim::INTERFACE_TYPE_TO_STRING.at(sim::InterfaceType::DISCRETE),
            sim::INTERFACE_TYPE_TO_STRING.at(sim::InterfaceType::CONTINUOUS));
        sim::SimUtilities::quit();
    }
    sim::S()->setInterfaceType(sim::STRING_TO_INTERFACE_TYPE.at(g_algorithm->interfaceType()));
}
