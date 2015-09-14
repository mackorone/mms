#include "Driver.h"

#include <fontstash/fontstash.h>
#include <tdogl/Bitmap.h>
#include <tdogl/Shader.h>
#include <thread>

#include "../mouse/MouseAlgorithms.h"
#include "Assert.h"
#include "Directory.h"
#include "GraphicUtilities.h"
#include "InterfaceType.h"
#include "Key.h"
#include "Logging.h"
#include "Maze.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "TriangleGraphic.h"
#include "units/Seconds.h"

namespace sim {

// Definition of the static variables for linking
Mouse* Driver::m_mouse;
MazeGraphic* Driver::m_mazeGraphic;
MouseGraphic* Driver::m_mouseGraphic;
MouseInterface* Driver::m_mouseInterface;
World* Driver::m_world;
IMouseAlgorithm* Driver::m_algorithm;
TextDrawer* Driver::m_textDrawer;
tdogl::Program* Driver::m_polygonProgram;
GLuint Driver::m_polygonVertexArrayObjectId;
GLuint Driver::m_polygonVertexBufferObjectId;
tdogl::Texture* Driver::m_textureAtlas;
tdogl::Program* Driver::m_textureProgram;
GLuint Driver::m_textureVertexArrayObjectId;
GLuint Driver::m_textureVertexBufferObjectId;

int rows = 32; // TODO: MACK
int cols = 64; // TODO: MACK

void Driver::drive(int argc, char* argv[]) {

    // Step -1: Make sure that this function is only ever called once
    static bool alreadyCalled = false;
    ASSERT(!alreadyCalled);
    alreadyCalled = true;

    // Step 0: Determine the runId, configure logging,
    // and initialize the State and Param objects.
    bootstrap();

    // Step 0.5: Remove any excessive archived runs
    SimUtilities::removeExcessArchivedRuns();

    // Step 1: Initialize the simulation objects
    initSimObjects();

    // Step 2: Initialize the graphics
    initGraphics(argc, argv);

    // Step 3: Initialize the mouse algorithm
    initMouseAlgo();

    // Step 4: Start the physics loop
    std::thread physicsThread([](){
        m_world->simulate();
    });

    // Step 5: Start the solving loop
    std::thread solvingThread([](){

        // Wait for the window to appear
        SimUtilities::sleep(Seconds(P()->glutInitDuration()));

        // Unfog the beginning tile if necessary
        if (S()->interfaceType() == InterfaceType::DISCRETE && P()->discreteInterfaceUnfogTileOnEntry()) {
            m_mazeGraphic->setTileFogginess(0, 0, false);
        }

        // Finally, begin execution of the mouse algorithm
        m_algorithm->solve(
            m_mazeGraphic->getWidth(),
            m_mazeGraphic->getHeight(),
            DIRECTION_TO_CHAR.at(STRING_TO_DIRECTION.at(P()->mouseStartingDirection())),
            m_mouseInterface);
    });

    // Step 6: Start the graphics loop
    glutMainLoop();
}

void Driver::bootstrap() {

    // First, determine the runId (just datetime, for now)
    std::string runId = SimUtilities::getDateTime();

    // Then we can initiliaze Logging
    Logging::initialize(runId);

    // Initialize the State object in order to:
    // 0) Set the runId
    // 1) Avoid a race condition (between threads)
    // 2) Register this thread as the main thread
    // 3) Initialize the Param object
    S()->setRunId(runId);
}

void Driver::initSimObjects() {
    Maze* maze = new Maze();
    m_mouse = new Mouse(maze);
    m_mazeGraphic = new MazeGraphic(maze);
    m_mouseGraphic = new MouseGraphic(m_mouse);
    m_mouseInterface = new MouseInterface(maze, m_mouse, m_mazeGraphic);
    m_world = new World(maze, m_mouse);
}

// loads a triangle into the VAO global
void Driver::LoadTriangle() {

    // make and bind the VAO
    glGenVertexArrays(1, &m_textureVertexArrayObjectId);
    glBindVertexArray(m_textureVertexArrayObjectId);
    
    // make and bind the VBO
    glGenBuffers(1, &m_textureVertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureVertexBufferObjectId);

    // Put the triangle vertices (XYZ) and texture coordinates (UV) into the VBO
    GLfloat vertexData[24 * rows * cols];
    for (int i = 0; i < rows; i += 1) {
        for (int j = 0; j < cols; j += 1) {
            VertexTexture p1 { // LL
                j * 3.0 / float(cols), i * 3.0 / float(rows), 0.0, 0.0,
            };
            VertexTexture p2 { // UL
                j * 3.0 / float(cols), (i + 1) * 3.0 / float(rows), 0.0, 1.0,
            };
            VertexTexture p3 { // UR
                (j + 1) * 3.0 / float(cols), (i + 1) * 3.0 / float(rows), 1.0, 1.0,
            };
            VertexTexture p4 { // LR
                (j + 1) * 3.0 / float(cols), i * 3.0 / float(rows), 1.0, 0.0,
            };
            TriangleTexture t1 = { p1, p2, p3 };
            TriangleTexture t2 = { p1, p3, p4 };
            GraphicUtilities::TEXTURE_CPU_BUFFER.push_back(t1);
            GraphicUtilities::TEXTURE_CPU_BUFFER.push_back(t2);
        }
    }

    // connect the xyz to the "coordinate" attribute of the vertex shader
    glEnableVertexAttribArray(m_textureProgram->attrib("coordinate"));
    glVertexAttribPointer(m_textureProgram->attrib("coordinate"), 2, GL_DOUBLE, GL_FALSE, 4*sizeof(double), NULL);
        
    // connect the uv coords to the "textureCoordinate" attribute of the vertex shader
    glEnableVertexAttribArray(m_textureProgram->attrib("textureCoordinate"));
    glVertexAttribPointer(m_textureProgram->attrib("textureCoordinate"), 2, GL_DOUBLE, GL_TRUE,  4*sizeof(double), (const GLvoid*)(2 * sizeof(double)));

    // unbind the VAO
    glBindVertexArray(0);
}

void Driver::draw() {

    // In order to ensure we're sleeping the correct amount of time, we time
    // the drawing operation and take it into account when we sleep.
    double start(SimUtilities::getHighResTime());

    // Determine the starting index of the mouse
    static const int mouseTrianglesStartingIndex = GraphicUtilities::GRAPHIC_CPU_BUFFER.size();

    // Make space for mouse updates and copy to the CPU buffer
    GraphicUtilities::GRAPHIC_CPU_BUFFER.erase(
        GraphicUtilities::GRAPHIC_CPU_BUFFER.begin() + mouseTrianglesStartingIndex,
        GraphicUtilities::GRAPHIC_CPU_BUFFER.end());

    // Fill the CPU buffer with new mouse triangles
    m_mouseGraphic->draw();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable scissoring so that the maps are only draw in specified locations.
    // Additionally, get the sizes and positions of each of the maps.
    glEnable(GL_SCISSOR_TEST);
    std::pair<int, int> fullMapPosition = GraphicUtilities::getFullMapPosition();
    std::pair<int, int> fullMapSize = GraphicUtilities::getFullMapSize();
    std::pair<int, int> zoomedMapPosition = GraphicUtilities::getZoomedMapPosition();
    std::pair<int, int> zoomedMapSize = GraphicUtilities::getZoomedMapSize();

    // ----- Drawing the polygons ----- //

    // Enable
    m_polygonProgram->use();
    glBindVertexArray(m_polygonVertexArrayObjectId);

    // Clear the vertex buffer object and copy over the CPU buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_polygonVertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, GraphicUtilities::GRAPHIC_CPU_BUFFER.size() * sizeof(TriangleGraphic), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, GraphicUtilities::GRAPHIC_CPU_BUFFER.size() * sizeof(TriangleGraphic),
        &GraphicUtilities::GRAPHIC_CPU_BUFFER.front());

    // Render the full map
    glScissor(fullMapPosition.first, fullMapPosition.second, fullMapSize.first, fullMapSize.second);
    m_polygonProgram->setUniformMatrix4("transformationMatrix",
        &GraphicUtilities::getFullMapTransformationMatrix().front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, 0, 3 * GraphicUtilities::GRAPHIC_CPU_BUFFER.size());

    // Render the zoomed map
    glScissor(zoomedMapPosition.first, zoomedMapPosition.second, zoomedMapSize.first, zoomedMapSize.second);
    m_polygonProgram->setUniformMatrix4("transformationMatrix",
        &GraphicUtilities::getZoomedMapTransformationMatrix(
            m_mouse->getInitialTranslation(), m_mouse->getCurrentTranslation(),
            m_mouse->getCurrentRotation()).front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, 0, 3 * GraphicUtilities::GRAPHIC_CPU_BUFFER.size());

    // Disable
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    m_polygonProgram->stopUsing();

    // ----- Drawing the textures ----- //

    // Enable
    m_textureProgram->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureAtlas->object());
    glBindVertexArray(m_textureVertexArrayObjectId);
    m_textureProgram->setUniform("_texture", 0);

    // Bind and fill the buffer with data
    glBindBuffer(GL_ARRAY_BUFFER, m_textureVertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, GraphicUtilities::TEXTURE_CPU_BUFFER.size() * sizeof(TriangleTexture), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, GraphicUtilities::TEXTURE_CPU_BUFFER.size() * sizeof(TriangleTexture),
        &GraphicUtilities::TEXTURE_CPU_BUFFER.front());

    // Render the texture on the full map
    glScissor(fullMapPosition.first, fullMapPosition.second, fullMapSize.first, fullMapSize.second);
    m_textureProgram->setUniformMatrix4("transformationMatrix",
        &GraphicUtilities::getFullMapTransformationMatrix().front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, 0, 3 * GraphicUtilities::TEXTURE_CPU_BUFFER.size());

    // Render the texture on the zoomed map
    glScissor(zoomedMapPosition.first, zoomedMapPosition.second, zoomedMapSize.first, zoomedMapSize.second);
    m_textureProgram->setUniformMatrix4("transformationMatrix",
        &GraphicUtilities::getZoomedMapTransformationMatrix(
            m_mouse->getInitialTranslation(), m_mouse->getCurrentTranslation(),
            m_mouse->getCurrentRotation()).front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, 0, 3 * GraphicUtilities::TEXTURE_CPU_BUFFER.size());
    
    // Disable
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    m_textureProgram->stopUsing();

    // ----- Drawing the text ----- //

    // TODO: MACK - Font-stash drawing
    m_textDrawer->commenceDrawingTextForFrame();
    auto size = GraphicUtilities::getWindowSize();
    //m_textDrawer->drawText(0, 0, "01234");
    //m_textDrawer->drawText(0, 470.0/2.0, "ABCDj");
    m_textDrawer->concludeDrawingTextForFrame();

    // We disable scissoring so that the glClear can take effect
    glDisable(GL_SCISSOR_TEST);

    // Display the result
    glutSwapBuffers();

    // Get the duration of the drawing operation, in seconds. Note that this duration
    // is simply the total number of real seconds that have passed, which is exactly
    // what we want (since the frame-rate is perceived in real-time and not CPU time).
    double end(SimUtilities::getHighResTime());
    double duration = end - start;

    // Notify the user of a late frame
    if (duration > 1.0/P()->frameRate()) {
        IF_PRINT_ELSE_LOG(P()->printLateFrames(), WARN,
            "A frame was late by %v seconds, which is %v percent late.",
            duration - 1.0/P()->frameRate(),
            (duration - 1.0/P()->frameRate())/(1.0/P()->frameRate()) * 100);
    }

    // Sleep the appropriate amount of time, base on the drawing duration
    SimUtilities::sleep(Seconds(std::max(0.0, 1.0/P()->frameRate() - duration)));
}

void Driver::keyPress(unsigned char key, int x, int y) {

    if (key == 'p') {
        // Pause the simulation (only in discrete mode)
        S()->setPaused(!S()->paused());
    }
    else if (key == 'f') {
        // Faster (only in discrete mode)
        S()->setSimSpeed(S()->simSpeed() * 1.5);
    }
    else if (key == 's') {
        // Slower (only in discrete mode)
        S()->setSimSpeed(S()->simSpeed() / 1.5);
    }
    else if (key == 'l') {
        // Cycle through the available layouts
        S()->setLayout(LAYOUT_CYCLE.at(S()->layout()));
    }
    else if (key == 'r') {
        // Toggle rotate zoomed map, but only if zoomed map is visible
        if (S()->layout() != Layout::FULL) {
            S()->setRotateZoomedMap(!S()->rotateZoomedMap());
        }
    }
    else if (key == 'i') {
        // Zoom in, but only if zoomed map is visible
        if (S()->layout() != Layout::FULL) {
            S()->setZoomedMapScale(S()->zoomedMapScale() * 1.5);
        }
    }
    else if (key == 'o') {
        // Zoom out, but only if zoomed map is visible
        if (S()->layout() != Layout::FULL) {
            S()->setZoomedMapScale(S()->zoomedMapScale() / 1.5);
        }
    }
    else if (key == 't') {
        // Toggle wall truth visibility
        S()->setWallTruthVisible(!S()->wallTruthVisible());
        m_mazeGraphic->updateWalls();
    }
    else if (key == 'c') {
        // Toggle tile colors
        S()->setTileColorsVisible(!S()->tileColorsVisible());
        m_mazeGraphic->updateColor();
    }
    else if (key == 'x') {
        // Toggle tile text
        S()->setTileTextVisible(!S()->tileTextVisible());
    }
    else if (key == 'g') {
        // Toggle tile fog
        S()->setTileFogVisible(!S()->tileFogVisible());
        m_mazeGraphic->updateFog();
    }
    else if (key == 'w') {
        // Toggle wireframe mode
        S()->setWireframeMode(!S()->wireframeMode());
        glPolygonMode(GL_FRONT_AND_BACK, S()->wireframeMode() ? GL_LINE : GL_FILL);
    }
    else if (key == 'q') {
        // Quit
        SimUtilities::quit();
    }
    else if (std::string("0123456789").find(key) != std::string::npos) {
        // Press an input button
        int inputButton = std::string("0123456789").find(key);
        S()->setInputButtonWasPressed(inputButton, true);
    }
}

void Driver::specialKeyPress(int key, int x, int y) {
    if (!SimUtilities::mapContains(INT_TO_KEY, key)) {
        return;
    }
    if (SimUtilities::vectorContains(ARROW_KEYS, INT_TO_KEY.at(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.at(key), true);
    }
}

void Driver::specialKeyRelease(int key, int x, int y) {
    if (!SimUtilities::mapContains(INT_TO_KEY, key)) {
        return;
    }
    if (SimUtilities::vectorContains(ARROW_KEYS, INT_TO_KEY.at(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.at(key), false);
    }
}

void Driver::initPolygonProgram() {

    // Generate the polygon vertex array object and vertex buffer object
    glGenVertexArrays(1, &m_polygonVertexArrayObjectId);
    glBindVertexArray(m_polygonVertexArrayObjectId);
    glGenBuffers(1, &m_polygonVertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, m_polygonVertexBufferObjectId);

    // Set up the program and attribute pointers
    m_polygonProgram = new tdogl::Program({tdogl::Shader::shaderFromFile(
        Directory::getResShadersDirectory() + "polygonVertexShader.txt", GL_VERTEX_SHADER)});
    glEnableVertexAttribArray(m_polygonProgram->attrib("coordinate"));
    // TODO: MACK sizeof(TriangleGraphic)
    glVertexAttribPointer(m_polygonProgram->attrib("coordinate"), 2, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), 0);
    glEnableVertexAttribArray(m_polygonProgram->attrib("color"));
    glVertexAttribPointer(m_polygonProgram->attrib("color"), 4, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (char*) NULL + 2 * sizeof(double));

    // Unbind the vertex array object and vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Driver::initTextureProgram() {

    // Generate the texture vertex array object and vertex buffer object
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(
        Directory::getResShadersDirectory() + "textureVertexShader.txt", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(
        Directory::getResShadersDirectory() + "textureFragmentShader.txt", GL_FRAGMENT_SHADER));
    m_textureProgram = new tdogl::Program(shaders);

    // Load the bitmap texture into the texture atlas
    tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(Directory::getResTexturesDirectory() + "hazard.png");
    bmp.flipVertically();
    m_textureAtlas = new tdogl::Texture(bmp);

    // TODO
    LoadTriangle();
}

void Driver::initGraphics(int argc, char* argv[]) {

    // GLUT Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(P()->defaultWindowWidth(), P()->defaultWindowHeight());
    GraphicUtilities::setWindowSize(P()->defaultWindowWidth(), P()->defaultWindowHeight());
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
    glPolygonMode(GL_FRONT_AND_BACK, S()->wireframeMode() ? GL_LINE : GL_FILL);

    // When the window changes size, notify the graphic utilities
    glutReshapeFunc([](int width, int height){
        glViewport(0,0, width, height);
        GraphicUtilities::setWindowSize(width, height);
    }); 

    // GLEW Initialization
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        PRINT(ERROR, "Unable to initialize GLEW.");
        SimUtilities::quit();
    }
    
    // Initialize the text drawer object // TODO: MACK - get the font from param
    // TODO: MACK - If the font doesn't exist, we silently fail and draw no text whatsoever
    m_textDrawer = new TextDrawer("Hack-Regular.ttf", 470.0 / 2.0);

    // Initialize the polygon drawing program
    initPolygonProgram();

    // Initialize the texture drawing program
    initTextureProgram();

    // Lastly, initially populate the vertex buffer object with tile information
    m_mazeGraphic->draw();
}

void Driver::initMouseAlgo() {

    // First, check to ensure that the mouse algorithm is valid
    if (!MouseAlgorithms::isMouseAlgorithm(P()->mouseAlgorithm())) {
        // TODO: MACK - make a note about the file where it needs to be declared
        PRINT(ERROR, "\"%v\" is not a valid mouse algorithm.",
            P()->mouseAlgorithm());
        SimUtilities::quit();
    }
    m_algorithm = MouseAlgorithms::getMouseAlgorithm(P()->mouseAlgorithm());

    // Initialize the mouse with the file provided
    std::string mouseFile = m_algorithm->mouseFile();
    bool success = m_mouse->initialize(mouseFile);
    if (!success) {
        PRINT(ERROR,
            "Unable to successfully initialize the mouse in the algorithm "
            "\"%v\" from \"%v\".",
            P()->mouseAlgorithm(),
            mouseFile);
        SimUtilities::quit();
    }

    // Initialize the interface type
    if (!SimUtilities::mapContains(STRING_TO_INTERFACE_TYPE, m_algorithm->interfaceType())) {
        PRINT(ERROR,
            "\"%v\" is not a valid interface type. You must declare the "
            "interface type of the mouse algorithm \"%v\" to be either \"%v\" "
            "or \"%v\".",
            m_algorithm->interfaceType(),
            P()->mouseAlgorithm(),
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE),
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::CONTINUOUS));
        SimUtilities::quit();
    }
    S()->setInterfaceType(STRING_TO_INTERFACE_TYPE.at(m_algorithm->interfaceType()));
}

} // namespace sim
