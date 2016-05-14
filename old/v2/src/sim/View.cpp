#include "View.h"

#include <tdogl/Bitmap.h>
#include <tdogl/Shader.h>

#include "../mouse/IMouseAlgorithm.h"
#include "BufferInterface.h"
#include "Directory.h"
#include "Layout.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "TransformationMatrix.h"
#include "units/Seconds.h"

namespace sim {

View::View(Model* model, int argc, char* argv[], const GlutFunctions& functions) : m_model(model) {

    m_bufferInterface = new BufferInterface(
        std::make_pair(m_model->getMaze()->getWidth(), m_model->getMaze()->getHeight()),
        &m_graphicCpuBuffer,
        &m_textureCpuBuffer
    );

    m_mazeGraphic = new MazeGraphic(model->getMaze(), m_bufferInterface);
    m_mouseGraphic = new MouseGraphic(model->getMouse(), m_bufferInterface);

    initGraphics(argc, argv, functions);
    initPolygonProgram();
    initTextureProgram();

    m_screenPixelsPerMeter = glutGet(GLUT_SCREEN_WIDTH) / (glutGet(GLUT_SCREEN_WIDTH_MM) / 1000.0);
    m_fontImageMap = getFontImageMap();
    m_header = new Header(model);
}

MazeGraphic* View::getMazeGraphic() {
    return m_mazeGraphic;
}

MouseGraphic* View::getMouseGraphic() {
    return m_mouseGraphic;
}

void View::setMouseAlgorithmAndOptions(
        IMouseAlgorithm* mouseAlgorithm,
        StaticMouseAlgorithmOptions options) {
    m_mouseAlgorithm = mouseAlgorithm;
    m_options = options;
    m_header->setMouseAlgorithmAndOptions(mouseAlgorithm, options);
}

void View::refresh() {

    // In order to ensure we're sleeping the correct amount of time, we time
    // the drawing operation and take it into account when we sleep.
    double start(SimUtilities::getHighResTimestamp());

    // First, clear fog as necessary
    if (m_mouseAlgorithm->automaticallyClearFog()) {
        // TODO: upforgrabs
        // This won't work if the mouse is traveling too quickly and travels more
        // than one tile per frame. Figure out a way that will work in that case.
        std::pair<int, int> currentPosition =
            m_model->getMouse()->getCurrentDiscretizedTranslation();
        m_mazeGraphic->setTileFogginess(currentPosition.first, currentPosition.second, false);
    }

    // Determine the starting index of the mouse
    static const int mouseTrianglesStartingIndex = m_graphicCpuBuffer.size();

    // Get the current mouse translation and rotation
    Cartesian currentMouseTranslation = m_model->getMouse()->getCurrentTranslation();
    Radians currentMouseRotation = m_model->getMouse()->getCurrentRotation();

    // Make space for mouse updates and fill the CPU buffer with new mouse triangles
    m_graphicCpuBuffer.erase(
        m_graphicCpuBuffer.begin() + mouseTrianglesStartingIndex,
        m_graphicCpuBuffer.end());
    getMouseGraphic()->draw(currentMouseTranslation, currentMouseRotation);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable scissoring so that the maps are only draw in specified locations.
    glEnable(GL_SCISSOR_TEST);

    // Re-populate both vertex buffer objects and then draw the tiles, the tile text, and then the mouse
    repopulateVertexBufferObjects();
    drawFullAndZoomedMaps(currentMouseTranslation, currentMouseRotation,
        m_polygonProgram, m_polygonVertexArrayObjectId, 0, 3 * mouseTrianglesStartingIndex);
    drawFullAndZoomedMaps(currentMouseTranslation, currentMouseRotation,
        m_textureProgram, m_textureVertexArrayObjectId, 0, 3 * m_textureCpuBuffer.size());
    drawFullAndZoomedMaps(currentMouseTranslation, currentMouseRotation,
        m_polygonProgram, m_polygonVertexArrayObjectId, 3 * mouseTrianglesStartingIndex,
        3 * (m_graphicCpuBuffer.size() - mouseTrianglesStartingIndex));

    // Disable scissoring so that the glClear can take effect, and so that
    // drawn text isn't clipped at all
    glDisable(GL_SCISSOR_TEST);

    // Draw the window header
    m_header->draw();

    // Display the result
    glutSwapBuffers();

    // Get the duration of the drawing operation, in seconds. Note that this duration
    // is simply the total number of real seconds that have passed, which is exactly
    // what we want (since the frame-rate is perceived in real-time and not CPU time).
    double end(SimUtilities::getHighResTimestamp());
    double duration = end - start;

    // Notify the user of a late frame
    if (P()->printLateFrames() && duration > 1.0/P()->frameRate()) {
        L()->warn(
            "A frame was late by %v seconds, which is %v percent late.",
            duration - 1.0/P()->frameRate(),
            (duration - 1.0/P()->frameRate())/(1.0/P()->frameRate()) * 100);
    }

    // Sleep the appropriate amount of time, base on the drawing duration
    SimUtilities::sleep(Seconds(std::max(0.0, 1.0/P()->frameRate() - duration)));
}

void View::updateWindowSize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    m_header->updateWindowSize(width, height);
    glViewport(0, 0, width, height);
}

std::set<char> View::getAllowableTileTextCharacters() {
    return ContainerUtilities::keys(m_fontImageMap);
}

void View::initTileGraphicText() {
    // Initialze the tile text in the buffer class, do caching for speed improvement
    m_bufferInterface->initTileGraphicText(
        Meters(P()->wallLength()),
        Meters(P()->wallWidth()),
        std::make_pair(
            m_options.tileTextNumberOfRows,
            m_options.tileTextNumberOfCols
        ),
        m_fontImageMap,
        P()->tileTextBorderFraction(),
        STRING_TO_TILE_TEXT_ALIGNMENT.at(P()->tileTextAlignment()));
}

void View::keyPress(unsigned char key, int x, int y) {

    // NOTE: If you're adding or removing anything from this function, make
    // sure to update wiki/Keys.md

    if (key == 'p') {
        // Toggle pause (only in discrete mode)
        if (STRING_TO_INTERFACE_TYPE.at(m_options.interfaceType) == InterfaceType::DISCRETE) {
            S()->setPaused(!S()->paused());
        }
        else {
            L()->warn(
                "Pausing the simulator is only allowed in %v mode.",
                INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE));
        }
    }
    else if (key == 'f') {
        // Faster (only in discrete mode)
        if (STRING_TO_INTERFACE_TYPE.at(m_options.interfaceType) == InterfaceType::DISCRETE) {
            S()->setSimSpeed(S()->simSpeed() * 1.5);
        }
        else {
            L()->warn(
                "Increasing the simulator speed is only allowed in %v mode.",
                INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE));
        }
    }
    else if (key == 's') {
        // Slower (only in discrete mode)
        if (STRING_TO_INTERFACE_TYPE.at(m_options.interfaceType) == InterfaceType::DISCRETE) {
            S()->setSimSpeed(S()->simSpeed() / 1.5);
        }
        else {
            L()->warn(
                "Decreasing the simulator speed is only allowed in %v mode.",
                INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE));
        }
    }
    else if (key == 'l') {
        // Cycle through the available layouts
        S()->setLayoutType(LAYOUT_TYPE_CYCLE.at(S()->layoutType()));
    }
    else if (key == 'r') {
        // Toggle rotate zoomed map
        S()->setRotateZoomedMap(!S()->rotateZoomedMap());
    }
    else if (key == 'i') {
        // Zoom in
        S()->setZoomedMapScale(S()->zoomedMapScale() * 1.5);
    }
    else if (key == 'o') {
        // Zoom out
        S()->setZoomedMapScale(S()->zoomedMapScale() / 1.5);
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
    else if (key == 'g') {
        // Toggle tile fog
        S()->setTileFogVisible(!S()->tileFogVisible());
        m_mazeGraphic->updateFog();
    }
    else if (key == 'x') {
        // Toggle tile text
        S()->setTileTextVisible(!S()->tileTextVisible());
        m_mazeGraphic->updateText();
    }
    else if (key == 'd') {
        // Toggle tile distance visibility
        S()->setTileDistanceVisible(!S()->tileDistanceVisible());
        m_mazeGraphic->updateText();
    }
    else if (key == 'h') {
        // Toggle header visibility
        S()->setHeaderVisible(!S()->headerVisible());
        m_header->updateLinesAndColumnStartingPositions();
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
        if (!S()->inputButtonWasPressed(inputButton)) {
            S()->setInputButtonWasPressed(inputButton, true);
            L()->info("Input button %v was pressed.", inputButton);
        }
        else {
            L()->warn(
                "Input button %v has not yet been acknowledged as pressed; pressing it has no effect.",
                inputButton);
        }
    }
}

void View::specialKeyPress(int key, int x, int y) {
    if (!ContainerUtilities::mapContains(INT_TO_KEY, key)) {
        return;
    }
    if (ContainerUtilities::vectorContains(ARROW_KEYS, INT_TO_KEY.at(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.at(key), true);
    }
}

void View::specialKeyRelease(int key, int x, int y) {
    if (!ContainerUtilities::mapContains(INT_TO_KEY, key)) {
        return;
    }
    if (ContainerUtilities::vectorContains(ARROW_KEYS, INT_TO_KEY.at(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.at(key), false);
    }
}

void View::initGraphics(int argc, char* argv[], const GlutFunctions& functions) {

    // GLUT Initialization
    glutInit(&argc, argv);
    // XXX: Is this necessary on OSX?
    // glutInitDisplayMode(2048 | GLUT_DOUBLE | GLUT_RGBA);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(P()->defaultWindowWidth(), P()->defaultWindowHeight());
    glutCreateWindow("Micromouse Simulator");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, S()->wireframeMode() ? GL_LINE : GL_FILL);
    glutDisplayFunc(functions.refresh);
    glutIdleFunc(functions.refresh);
    glutReshapeFunc(functions.windowResize);
    glutKeyboardFunc(functions.keyPress);
    glutSpecialFunc(functions.specialKeyPress);
    glutSpecialUpFunc(functions.specialKeyRelease);

    // XXX: If this necessary on OSX?
    // glewExperimental = GL_TRUE;

    // GLEW Initialization
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        L()->error("Unable to initialize GLEW.");
        SimUtilities::quit();
    }

    // XXX: Print out the OpenGL version
    // std::cout << glGetString(GL_VERSION) << std::endl;
}

void View::initPolygonProgram() {

    // Generate the polygon vertex array object and vertex buffer object
    glGenVertexArrays(1, &m_polygonVertexArrayObjectId);
    glBindVertexArray(m_polygonVertexArrayObjectId);
    glGenBuffers(1, &m_polygonVertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, m_polygonVertexBufferObjectId);

    // Set up the program and attribute pointers
    m_polygonProgram = new tdogl::Program({tdogl::Shader::shaderFromFile(
        Directory::getResShadersDirectory() + "polygonVertexShader.txt", GL_VERTEX_SHADER)});
    glEnableVertexAttribArray(m_polygonProgram->attrib("coordinate"));
    glVertexAttribPointer(m_polygonProgram->attrib("coordinate"),
        2, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), 0);
    glEnableVertexAttribArray(m_polygonProgram->attrib("color"));
    glVertexAttribPointer(m_polygonProgram->attrib("color"),
        4, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (char*) NULL + 2 * sizeof(double));

    // Unbind the vertex array object and vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void View::initTextureProgram() {

    // Generate the texture vertex array object and vertex buffer object
    glGenVertexArrays(1, &m_textureVertexArrayObjectId);
    glBindVertexArray(m_textureVertexArrayObjectId);
    glGenBuffers(1, &m_textureVertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureVertexBufferObjectId);

    // Set up the program and attribute pointers
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(
        Directory::getResShadersDirectory() + "textureVertexShader.txt", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(
        Directory::getResShadersDirectory() + "textureFragmentShader.txt", GL_FRAGMENT_SHADER));
    m_textureProgram = new tdogl::Program(shaders);
    glEnableVertexAttribArray(m_textureProgram->attrib("coordinate"));
    glVertexAttribPointer(m_textureProgram->attrib("coordinate"),
        2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), NULL);
    glEnableVertexAttribArray(m_textureProgram->attrib("textureCoordinate"));
    glVertexAttribPointer(m_textureProgram->attrib("textureCoordinate"),
        2, GL_DOUBLE, GL_TRUE, 4 * sizeof(double), (char*) NULL + 2 * sizeof(double));

    // Load the bitmap texture into the texture atlas
    std::string tileTextFontImagePath = Directory::getResImgsDirectory() + P()->tileTextFontImage();
    if (!SimUtilities::isFile(tileTextFontImagePath)) {
        L()->error(
            "Could not find font image file \"%v\" in \"%v\".",
            P()->tileTextFontImage(),
            Directory::getResImgsDirectory());
        SimUtilities::quit();
    }
    tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(tileTextFontImagePath);
    bmp.flipVertically();
    m_textureAtlas = new tdogl::Texture(bmp);

    // Unbind the vertex array object and vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

std::map<char, std::pair<double, double>> View::getFontImageMap() {

    // These values must perfectly reflect the font image being used, or else
    // the wrong characters will be displayed on the tiles.
    const std::string fontImageChars =
        " !\"#$%&'()*+,-./0123456789:;<=>?"
        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
        "`abcdefghijklmnopqrstuvwxyz{|}~";

    // Get a map of the font image characters (allowable tile text characters)
    // to their position in the png image (a fraction from 0.0 to 1.0)
    std::map<char, std::pair<double, double>> fontImageMap;
    for (int i = 0; i < fontImageChars.size(); i += 1) {
        fontImageMap.insert(
            std::make_pair(
                fontImageChars.at(i),
                std::make_pair(
                    static_cast<double>(i + 0) / static_cast<double>(fontImageChars.size()),
                    static_cast<double>(i + 1) / static_cast<double>(fontImageChars.size())
                )
            )
        );
    }

    // Return the map
    return fontImageMap;
}

void View::repopulateVertexBufferObjects() {

    // Clear the polygon vertex buffer object and re-populate it with data
    glBindBuffer(GL_ARRAY_BUFFER, m_polygonVertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, m_graphicCpuBuffer.size() * sizeof(TriangleGraphic), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_graphicCpuBuffer.size() * sizeof(TriangleGraphic),
        &m_graphicCpuBuffer.front());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Clear the texture vertex buffer object and re-populate it with data
    glBindBuffer(GL_ARRAY_BUFFER, m_textureVertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, m_textureCpuBuffer.size() * sizeof(TriangleTexture), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_textureCpuBuffer.size() * sizeof(TriangleTexture),
        &m_textureCpuBuffer.front());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void View::drawFullAndZoomedMaps(
        const Coordinate& currentMouseTranslation, const Angle& currentMouseRotation,
        tdogl::Program* program, int vaoId, int vboStartingIndex, int vboEndingIndex) {

    // Get the sizes and positions of each of the maps.
    std::pair<int, int> fullMapPosition = Layout::getFullMapPosition(
        m_windowWidth, m_windowHeight, m_header->getHeight(), P()->windowBorderWidth(), S()->layoutType());
    std::pair<int, int> fullMapSize = Layout::getFullMapSize(
        m_windowWidth, m_windowHeight, m_header->getHeight(), P()->windowBorderWidth(), S()->layoutType());
    std::pair<int, int> zoomedMapPosition = Layout::getZoomedMapPosition(
        m_windowWidth, m_windowHeight, m_header->getHeight(), P()->windowBorderWidth(), S()->layoutType());
    std::pair<int, int> zoomedMapSize = Layout::getZoomedMapSize(
        m_windowWidth, m_windowHeight, m_header->getHeight(), P()->windowBorderWidth(), S()->layoutType());

    // Get the physical size of the maze (in meters)
    double physicalMazeWidth = P()->wallWidth() + m_model->getMaze()->getWidth() * (P()->wallWidth() + P()->wallLength());
    double physicalMazeHeight = P()->wallWidth() + m_model->getMaze()->getHeight() * (P()->wallWidth() + P()->wallLength());
    std::pair<double, double> physicalMazeSize = std::make_pair(physicalMazeWidth, physicalMazeHeight);

    // Start using the program and vertex array object
    program->use();
    glBindVertexArray(vaoId);

    // If it's the texture program, bind the texture and set the uniform
    if (program == m_textureProgram) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureAtlas->object());
        program->setUniform("_texture", 0);
    }

    // Render the full map
    glScissor(fullMapPosition.first, fullMapPosition.second, fullMapSize.first, fullMapSize.second);
    program->setUniformMatrix4("transformationMatrix",
        &TransformationMatrix::getFullMapTransformationMatrix(
            Meters(P()->wallWidth()),
            physicalMazeSize,
            fullMapPosition,
            fullMapSize,
            std::make_pair(m_windowWidth, m_windowHeight)).front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, vboStartingIndex, vboEndingIndex);

    // Render the zoomed map
    glScissor(zoomedMapPosition.first, zoomedMapPosition.second, zoomedMapSize.first, zoomedMapSize.second);
    program->setUniformMatrix4("transformationMatrix",
        &TransformationMatrix::getZoomedMapTransformationMatrix(
            physicalMazeSize,
            zoomedMapPosition,
            zoomedMapSize,
            std::make_pair(m_windowWidth, m_windowHeight),
            m_screenPixelsPerMeter,
            S()->zoomedMapScale(),
            S()->rotateZoomedMap(),
            m_model->getMouse()->getInitialTranslation(),
            currentMouseTranslation,
            currentMouseRotation).front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, vboStartingIndex, vboEndingIndex);

    // Stop using the program and vertex array object
    glBindVertexArray(0);
    program->stopUsing();

    // If it's the texture program, we should additionally unbind the texture
    if (program == m_textureProgram) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

} // namespace sim
