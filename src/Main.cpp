#include <thread>

#include <fontstash/fontstash.h>
#include <glut/glut.h>
#include <tdogl/Bitmap.h>
#include <tdogl/Program.h>
#include <tdogl/Shader.h>
#include <tdogl/Texture.h>

#include "mouse/IMouseAlgorithm.h"
#include "mouse/MouseAlgorithms.h"
#include "sim/Directory.h"
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

// TODO: To hide more, put all of this in a class and just call a method of the class in main()

// Simulation objects
sim::Mouse*          mouse;
sim::MazeGraphic*    mazeGraphic;
sim::MouseGraphic*   mouseGraphic;
sim::MouseInterface* mouseInterface;
sim::World*          world;

// Micromouse algorithm object
IMouseAlgorithm* algorithm;

// Text drawing object
sim::TextDrawer* textDrawer;

// Polygon program variables
tdogl::Program* polygonProgram;
GLuint          polygonVertexArrayObjectId;
GLuint          polygonVertexBufferObjectId;

// Texture program variables
tdogl::Texture* textureAtlas;
tdogl::Program* textureProgram;
GLuint          textureVertexArrayObjectId;
GLuint          textureVertexBufferObjectId;
int rows = 2; // TODO: MACK
int cols = 3; // TODO: MACK

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
    mouse = new sim::Mouse(maze);
    mazeGraphic = new sim::MazeGraphic(maze);
    mouseGraphic = new sim::MouseGraphic(mouse);
    mouseInterface = new sim::MouseInterface(maze, mouse, mazeGraphic);
    world = new sim::World(maze, mouse);
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
            vertexData[15*cols*i + 15*j +  0] = (j * 1.0 / float(cols)) * 2 - 1; // X
            vertexData[15*cols*i + 15*j +  1] = (i * 1.0 / float(rows)) * 2 - 1; // Y
            vertexData[15*cols*i + 15*j +  2] = 0.0; // Z
            vertexData[15*cols*i + 15*j +  3] = 0.5; // U
            vertexData[15*cols*i + 15*j +  4] = 1.0; // V
            vertexData[15*cols*i + 15*j +  5] = (j * 1.0 / float(cols) + 0.5 / float(cols)) * 2 - 1; // X
            vertexData[15*cols*i + 15*j +  6] = ((i + 1) * 1.0 / float(rows)) * 2 - 1; // Y
            vertexData[15*cols*i + 15*j +  7] = 0.0; // Z
            vertexData[15*cols*i + 15*j +  8] = 0.0; // U
            vertexData[15*cols*i + 15*j +  9] = 0.0; // V
            vertexData[15*cols*i + 15*j + 10] = ((j + 1) * 1.0 / float(cols)) * 2 - 1; // X
            vertexData[15*cols*i + 15*j + 11] = (i * 1.0 / float(rows)) * 2 - 1; // Y
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

void draw() {

    // In order to ensure we're sleeping the correct amount of time, we time
    // the drawing operation and take it into account when we sleep.
    double start(sim::SimUtilities::getHighResTime());

    // Determine the starting index of the mouse
    static const int mouseTrianglesStartingIndex = sim::GraphicUtilities::TGB.size();

    // Make space for mouse updates and copy to the CPU buffer
    sim::GraphicUtilities::TGB.erase(sim::GraphicUtilities::TGB.begin() + mouseTrianglesStartingIndex, sim::GraphicUtilities::TGB.end());

    // Fill the CPU buffer with new mouse triangles
    mouseGraphic->draw();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // ----- Drawing the polygons ----- //

    // Enable scissoring so that the maps are only draw in specified locations
    glEnable(GL_SCISSOR_TEST);

    // Enable the program and vertex array object
    polygonProgram->use();
    glBindVertexArray(polygonVertexArrayObjectId);

    // Clear the vertex buffer object and copy over the CPU buffer
    glBindBuffer(GL_ARRAY_BUFFER, polygonVertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, sim::GraphicUtilities::TGB.size() * sizeof(sim::TriangleGraphic), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sim::GraphicUtilities::TGB.size() * sizeof(sim::TriangleGraphic),
        &sim::GraphicUtilities::TGB.front());

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
        mouse->getInitialTranslation(), mouse->getCurrentTranslation(), mouse->getCurrentRotation()).front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, 0, 3 * sim::GraphicUtilities::TGB.size());

    // Disable the program and unbind both the vertex buffer object and vertex array object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    polygonProgram->stopUsing();

    // We disable scissoring so that the glClear can take effect
    glDisable(GL_SCISSOR_TEST);

    // ----- Drawing the textures ----- //

// TODO: MACK
#if(1)

    // Bind the texture program
    textureProgram->use();
        
    // Bind the texture and vertex array object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlas->object());
    glBindVertexArray(textureVertexArrayObjectId);
    
    // Render the texture
    textureProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
    glDrawArrays(GL_TRIANGLES, 0, 15 * rows * cols);
    
    // Unbind the program and both the vertex array object and vertex buffer object
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    textureProgram->stopUsing();

    // ----- Drawing the text ----- //

    // TODO: MACK - Font-stash drawing
    textDrawer->commenceDrawingTextForFrame();
    auto size = sim::GraphicUtilities::getWindowSize();
    textDrawer->drawText(100, size.second - 100, "In the town where I was born, lived a man: 1234");
    textDrawer->concludeDrawingTextForFrame();

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
        mazeGraphic->updateWalls();
    }
    else if (key == 'c') {
        // Toggle tile colors
        sim::S()->setTileColorsVisible(!sim::S()->tileColorsVisible());
        mazeGraphic->updateColor();
    }
    else if (key == 'x') {
        // Toggle tile text
        sim::S()->setTileTextVisible(!sim::S()->tileTextVisible());
    }
    else if (key == 'g') {
        // Toggle tile fog
        sim::S()->setTileFogVisible(!sim::S()->tileFogVisible());
        mazeGraphic->updateFog();
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
    if (!sim::SimUtilities::mapContains(sim::INT_TO_KEY, key)) {
        return;
    }
    if (sim::SimUtilities::vectorContains(sim::ARROW_KEYS, sim::INT_TO_KEY.at(key))) {
        sim::S()->setArrowKeyIsPressed(sim::INT_TO_KEY.at(key), true);
    }
}

void specialKeyRelease(int key, int x, int y) {
    if (!sim::SimUtilities::mapContains(sim::INT_TO_KEY, key)) {
        return;
    }
    if (sim::SimUtilities::vectorContains(sim::ARROW_KEYS, sim::INT_TO_KEY.at(key))) {
        sim::S()->setArrowKeyIsPressed(sim::INT_TO_KEY.at(key), false);
    }
}

void initPolygonProgram() {

    // Generate the polygon vertex array object and vertex buffer object
    glGenVertexArrays(1, &polygonVertexArrayObjectId);
    glBindVertexArray(polygonVertexArrayObjectId);
    glGenBuffers(1,  &polygonVertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, polygonVertexBufferObjectId);

    // Set up the program and attribute pointers
    polygonProgram = new tdogl::Program({tdogl::Shader::shaderFromFile(
        sim::Directory::getResShadersDirectory() + "polygonVertexShader.txt", GL_VERTEX_SHADER)});
    glEnableVertexAttribArray(polygonProgram->attrib("coordinate"));
    glVertexAttribPointer(polygonProgram->attrib("coordinate"), 2, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), 0);
    glEnableVertexAttribArray(polygonProgram->attrib("color"));
    glVertexAttribPointer(polygonProgram->attrib("color"), 4, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (char*) NULL + 2 * sizeof(double));

    // Unbind the vertex array object and vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void initTextureProgram() {

    // Generate the texture vertex array object and vertex buffer object
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(
        sim::Directory::getResShadersDirectory() + "textureVertexShader.txt", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(
        sim::Directory::getResShadersDirectory() + "textureFragmentShader.txt", GL_FRAGMENT_SHADER));
    textureProgram = new tdogl::Program(shaders);

    // Load the bitmap texture into the texture atlas
    tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(sim::Directory::getResTexturesDirectory() + "hazard.png");
    bmp.flipVertically();
    textureAtlas = new tdogl::Texture(bmp);

    // TODO
    LoadTriangle();
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
    
    // Initialize the text drawer object // TODO: MACK - get the font from param
    textDrawer = new sim::TextDrawer("DroidSansMono.ttf", 30.0);

    // Initialize the polygon drawing program
    initPolygonProgram();

    // Initialize the texture drawing program
    initTextureProgram();

    // Lastly, initially populate the vertex buffer object with tile information
    mazeGraphic->draw();
}

void initMouseAlgo() {

    // First, check to ensure that the mouse algorithm is valid
    if (!MouseAlgorithms::isMouseAlgorithm(sim::P()->mouseAlgorithm())) {
        // TODO: MACK - make a note about the file where it needs to be declared
        PRINT(ERROR, "\"%v\" is not a valid mouse algorithm.",
            sim::P()->mouseAlgorithm());
        sim::SimUtilities::quit();
    }
    algorithm = MouseAlgorithms::getMouseAlgorithm(sim::P()->mouseAlgorithm());

    // Initialize the mouse with the file provided
    std::string mouseFile = algorithm->mouseFile();
    bool success = mouse->initialize(mouseFile);
    if (!success) {
        PRINT(ERROR,
            "Unable to successfully initialize the mouse in the algorithm "
            "\"%v\" from \"%v\".",
            sim::P()->mouseAlgorithm(),
            mouseFile);
        sim::SimUtilities::quit();
    }

    // Initialize the interface type
    if (!sim::SimUtilities::mapContains(sim::STRING_TO_INTERFACE_TYPE, algorithm->interfaceType())) {
        PRINT(ERROR,
            "\"%v\" is not a valid interface type. You must declare the "
            "interface type of the mouse algorithm \"%v\" to be either \"%v\" "
            "or \"%v\".",
            algorithm->interfaceType(),
            sim::P()->mouseAlgorithm(),
            sim::INTERFACE_TYPE_TO_STRING.at(sim::InterfaceType::DISCRETE),
            sim::INTERFACE_TYPE_TO_STRING.at(sim::InterfaceType::CONTINUOUS));
        sim::SimUtilities::quit();
    }
    sim::S()->setInterfaceType(sim::STRING_TO_INTERFACE_TYPE.at(algorithm->interfaceType()));
}

int main(int argc, char* argv[]) {

    // Step 0: Determine the runId, configure logging,
    // and initialize the State and Param objects.
    bootstrap();

    // Step 0.5: Remove any excessive archived runs
    sim::SimUtilities::removeExcessArchivedRuns();

    // Step 1: Initialize the simulation objects
    initSimObjects();

    // Step 2: Initialize the graphics
    initGraphics(argc, argv);

    // Step 3: Initialize the mouse algorithm
    initMouseAlgo();

    // Step 4: Start the physics loop
    std::thread physicsThread([](){
        world->simulate();
    });

    // Step 5: Start the solving loop
    std::thread solvingThread([](){

        // Wait for the window to appear
        sim::SimUtilities::sleep(sim::Seconds(sim::P()->glutInitDuration()));

        // Unfog the beginning tile if necessary
        if (sim::S()->interfaceType() == sim::InterfaceType::DISCRETE && sim::P()->discreteInterfaceUnfogTileOnEntry()) {
            mazeGraphic->setTileFogginess(0, 0, false);
        }

        // Finally, begin execution of the mouse algorithm
        algorithm->solve(
            mazeGraphic->getWidth(),
            mazeGraphic->getHeight(),
            sim::DIRECTION_TO_CHAR.at(sim::STRING_TO_DIRECTION.at(sim::P()->mouseStartingDirection())),
            mouseInterface);
    });

    // Step 6: Start the graphics loop
    sim::S()->enterMainLoop();
    glutMainLoop();
}
