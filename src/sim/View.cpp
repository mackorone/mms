#include "View.h"

#include <tdogl/Bitmap.h>
#include <tdogl/Shader.h>

#include "Assert.h"
#include "Directory.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "units/Seconds.h"

namespace sim {

// These values must perfectly reflect the font image being used
// TODO: MACK - make this into a map
const std::string FONT_IMAGE_CHARS =
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

View::View(Model* model, int argc, char* argv[], const GlutFunctions& functions) : m_model(model) {

    m_mazeGraphic = new MazeGraphic(model->getMaze(), this); // TODO: MACK
    m_mouseGraphic = new MouseGraphic(model->getMouse(), this); // TODO: MACK

    initGraphics(argc, argv, functions);
    initPolygonProgram();
    initTextureProgram();

    // TODO: MACK - If the font doesn't exist, we silently fail and draw no text whatsoever
    // Initialize the text drawer object // TODO: MACK - get the font from param
    m_textDrawer = new TextDrawer("Hack-Regular.ttf", 470.0 / 2.0, this);

    // Lastly, initially populate the vertex buffer object with tile information
    getMazeGraphic()->draw();
}

MazeGraphic* View::getMazeGraphic() {
    return m_mazeGraphic;
}

MouseGraphic* View::getMouseGraphic() {
    return m_mouseGraphic;
}

std::pair<int, int> View::getWindowSize() {
    return std::make_pair(m_windowWidth, m_windowHeight);
}

void View::draw() {

    // In order to ensure we're sleeping the correct amount of time, we time
    // the drawing operation and take it into account when we sleep.
    double start(SimUtilities::getHighResTime());

    // Determine the starting index of the mouse
    static const int mouseTrianglesStartingIndex = GRAPHIC_CPU_BUFFER.size();

    // Get the current mouse translation and rotation
    Cartesian currentMouseTranslation = m_model->getMouse()->getCurrentTranslation();
    Radians currentMouseRotation = m_model->getMouse()->getCurrentRotation();

    // Make space for mouse updates and fill the CPU buffer with new mouse triangles
    GRAPHIC_CPU_BUFFER.erase(
        GRAPHIC_CPU_BUFFER.begin() + mouseTrianglesStartingIndex,
        GRAPHIC_CPU_BUFFER.end());
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
        m_textureProgram, m_textureVertexArrayObjectId, 0, 3 * TEXTURE_CPU_BUFFER.size());
    drawFullAndZoomedMaps(currentMouseTranslation, currentMouseRotation,
        m_polygonProgram, m_polygonVertexArrayObjectId, 3 * mouseTrianglesStartingIndex,
        3 * (GRAPHIC_CPU_BUFFER.size() - mouseTrianglesStartingIndex));

    // Disable scissoring so that the glClear can take effect, and so that
    // drawn text isn't clipped at all
    glDisable(GL_SCISSOR_TEST);

    // ----- Drawing the text ----- //
    // TODO: MACK - Font-stash drawing
    m_textDrawer->commenceDrawingTextForFrame();
    //m_textDrawer->drawText(0, 0, "01234");
    m_textDrawer->concludeDrawingTextForFrame();
    // -------------

    // Display the result
    glutSwapBuffers();

    // Get the duration of the drawing operation, in seconds. Note that this duration
    // is simply the total number of real seconds that have passed, which is exactly
    // what we want (since the frame-rate is perceived in real-time and not CPU time).
    double end(SimUtilities::getHighResTime());
    double duration = end - start;

    // Notify the user of a late frame
    if (duration > 1.0/P()->frameRate()) {
        L()->warn(
            "A frame was late by %v seconds, which is %v percent late.",
            duration - 1.0/P()->frameRate(),
            (duration - 1.0/P()->frameRate())/(1.0/P()->frameRate()) * 100);
    }

    // Sleep the appropriate amount of time, base on the drawing duration
    SimUtilities::sleep(Seconds(std::max(0.0, 1.0/P()->frameRate() - duration)));
}

void View::setWindowSize(int width, int height) {
    ASSERT_LT(0, width);
    ASSERT_LT(0, height);
    m_windowWidth = width;
    m_windowHeight = height;
    glViewport(0, 0, width, height);
}

void View::initGraphics(int argc, char* argv[], const GlutFunctions& functions) {

    // GLUT Initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(P()->defaultWindowWidth(), P()->defaultWindowHeight());
    setWindowSize(P()->defaultWindowWidth(), P()->defaultWindowHeight());
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Micromouse Simulator");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glutDisplayFunc(functions.draw);
    glutIdleFunc(functions.draw);
    glutReshapeFunc(functions.windowResize);
    glutKeyboardFunc(functions.keyPress);
    glutSpecialFunc(functions.specialKeyPress);
    glutSpecialUpFunc(functions.specialKeyRelease);

    glPolygonMode(GL_FRONT_AND_BACK, S()->wireframeMode() ? GL_LINE : GL_FILL);

    // GLEW Initialization
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        L()->error("Unable to initialize GLEW.");
        SimUtilities::quit();
    }
    
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
    // TODO: MACK - Make the font image a parameter...
    tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(Directory::getResImgsDirectory() + "DroidSansMono.png");
    bmp.flipVertically();
    m_textureAtlas = new tdogl::Texture(bmp);

    // Unbind the vertex array object and vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void View::repopulateVertexBufferObjects() {

    // Clear the polygon vertex buffer object and re-populate it with data
    glBindBuffer(GL_ARRAY_BUFFER, m_polygonVertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, GRAPHIC_CPU_BUFFER.size() * sizeof(TriangleGraphic), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, GRAPHIC_CPU_BUFFER.size() * sizeof(TriangleGraphic),
        &GRAPHIC_CPU_BUFFER.front());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Clear the texture vertex buffer object and re-populate it with data
    glBindBuffer(GL_ARRAY_BUFFER, m_textureVertexBufferObjectId);
    glBufferData(GL_ARRAY_BUFFER, TEXTURE_CPU_BUFFER.size() * sizeof(TriangleTexture), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, TEXTURE_CPU_BUFFER.size() * sizeof(TriangleTexture),
        &TEXTURE_CPU_BUFFER.front());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void View::drawFullAndZoomedMaps(
        const Coordinate& currentMouseTranslation, const Angle& currentMouseRotation,
        tdogl::Program* program, int vaoId, int vboStartingIndex, int vboEndingIndex) {

    // Get the sizes and positions of each of the maps.
    std::pair<int, int> fullMapPosition = getFullMapPosition(m_windowWidth, m_windowHeight);
    std::pair<int, int> fullMapSize = getFullMapSize(m_windowWidth, m_windowHeight);
    std::pair<int, int> zoomedMapPosition = getZoomedMapPosition(m_windowWidth, m_windowHeight);
    std::pair<int, int> zoomedMapSize = getZoomedMapSize(m_windowWidth, m_windowHeight);

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
    // TODO: MACK - refactor this, make address operator easier to see/understand
    program->setUniformMatrix4("transformationMatrix",
        &getFullMapTransformationMatrix(
            m_model->getMaze()->getPhysicalSize(),
            fullMapPosition,
            fullMapSize).front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, vboStartingIndex, vboEndingIndex);

    // Render the zoomed map
    glScissor(zoomedMapPosition.first, zoomedMapPosition.second, zoomedMapSize.first, zoomedMapSize.second);
    program->setUniformMatrix4("transformationMatrix",
        &getZoomedMapTransformationMatrix(
            m_model->getMaze()->getPhysicalSize(),
            zoomedMapPosition,
            zoomedMapSize,
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

std::pair<int, int> View::getFullMapPosition(int windowWidth, int windowHeight) {
    return std::make_pair(P()->windowBorderWidth(), P()->windowBorderWidth());
}

std::pair<int, int> View::getZoomedMapPosition(int windowWidth, int windowHeight) {
    if (S()->layout() == Layout::BOTH) {
        return std::make_pair((windowWidth + P()->windowBorderWidth()) / 2, P()->windowBorderWidth());
    }
    return std::make_pair(P()->windowBorderWidth(), P()->windowBorderWidth());
}

std::pair<int, int> View::getFullMapSize(int windowWidth, int windowHeight) {
    int width = windowWidth - 2 * P()->windowBorderWidth();
    int height = windowHeight - 2 * P()->windowBorderWidth();
    if (S()->layout() == Layout::ZOOMED) {
        width = 0;
    }
    else if (S()->layout() == Layout::BOTH) {
        width = (width - P()->windowBorderWidth()) / 2;
    }
    return std::make_pair(width, height);
}

std::pair<int, int> View::getZoomedMapSize(int windowWidth, int windowHeight) {
    int width = windowWidth - 2 * P()->windowBorderWidth();
    int height = windowHeight - 2 * P()->windowBorderWidth();
    if (S()->layout() == Layout::FULL) {
        width = 0;
    }
    else if (S()->layout() == Layout::BOTH) {
        width = (width - P()->windowBorderWidth()) / 2;
    }
    return std::make_pair(width, height);
}

std::pair<double, double> View::mapPixelCoordinateToOpenGlCoordinate(
        double x, double y, int windowWidth, int windowHeight) {
    return std::make_pair(2 * x / windowWidth - 1, 2 * y / windowHeight - 1);
}

std::vector<float> View::getFullMapTransformationMatrix(
        std::pair<double, double> physicalMazeSize,
        std::pair<int, int> fullMapPosition,
        std::pair<int, int> fullMapSize) {

    // The purpose of this function is to produce a 4x4 matrix which, when
    // applied to the physical coordinate within the vertex shader, transforms
    // it into an OpenGL coordinate for the full map. In this case, the zoomed
    // map always contains the entirety of the maze.

    // Step 1: The physical point (0,0) corresponds to the middle of the
    // bottom-left corner piece:
    //                                 |       |
    //                                 +-------+---
    //                                 |       |
    //                                 |   X   |
    //                                 |       |
    //                                 +-------+---
    //
    // However, we want to make sure that the entire maze is visible within the
    // map window. To ensure this, we first have to translate the physical
    // positions so that (0,0) actually refers to the bottom-left corner of the
    // bottom-left corner:
    //
    //                                 |       |
    //                                 +-------+---
    //                                 |       |
    //                                 |       |
    //                                 |       |
    //                                 X-------+---
    //
    std::vector<float> initialTranslationMatrix = {
        1.0, 0.0, 0.0, static_cast<float>(0.5 * P()->wallWidth()),
        0.0, 1.0, 0.0, static_cast<float>(0.5 * P()->wallWidth()),
        0.0, 0.0, 1.0,                                        0.0,
        0.0, 0.0, 0.0,                                        1.0,
    };

    // Ensure that the maze width and height always appear equally scaled.
    double physicalWidth = physicalMazeSize.first;
    double physicalHeight = physicalMazeSize.second;

    // Note that this is not literally the number of pixels per meter of the
    // screen. Rather, it's our desired number of pixels per simulation meter.
    double pixelsPerMeter = std::min(fullMapSize.first / physicalWidth, fullMapSize.second / physicalHeight);
    double pixelWidth = pixelsPerMeter * physicalWidth;
    double pixelHeight = pixelsPerMeter * physicalHeight;

    std::pair<double, double> openGlOrigin = mapPixelCoordinateToOpenGlCoordinate(0, 0, getWindowSize().first, getWindowSize().second);
    std::pair<double, double> openGlMazeSize = mapPixelCoordinateToOpenGlCoordinate(pixelWidth, pixelHeight, getWindowSize().first, getWindowSize().second);
    double openGlWidth = openGlMazeSize.first - openGlOrigin.first;
    double openGlHeight = openGlMazeSize.second - openGlOrigin.second;

    double horizontalScaling = openGlWidth / physicalWidth;
    double verticalScaling = openGlHeight / physicalHeight;

    std::vector<float> scalingMatrix = {
        static_cast<float>(horizontalScaling),                                 0.0, 0.0, 0.0,
                                          0.0, static_cast<float>(verticalScaling), 0.0, 0.0,
                                          0.0,                                 0.0, 1.0, 0.0,
                                          0.0,                                 0.0, 0.0, 1.0,
    };
    
    // Step 3: Construct the translation matrix. Note that here we ensure that
    // the maze is centered within the map boundaries.
    double pixelLowerLeftCornerX = fullMapPosition.first + 0.5 * (fullMapSize.first - pixelWidth);
    double pixelLowerLeftCornerY = fullMapPosition.second + 0.5 * (fullMapSize.second - pixelHeight);
    std::pair<double, double> openGlLowerLeftCorner = mapPixelCoordinateToOpenGlCoordinate(
        pixelLowerLeftCornerX, pixelLowerLeftCornerY, getWindowSize().first, getWindowSize().second);
    std::vector<float> translationMatrix = {
        1.0, 0.0, 0.0,  static_cast<float>(openGlLowerLeftCorner.first),
        0.0, 1.0, 0.0, static_cast<float>(openGlLowerLeftCorner.second),
        0.0, 0.0, 1.0,                                              0.0,
        0.0, 0.0, 0.0,                                              1.0,
    };

    // Step 4: Compose the matrices
    std::vector<float> transformationMatrix =
        multiply4x4Matrices(translationMatrix,
        multiply4x4Matrices(scalingMatrix,
                            initialTranslationMatrix));
    return transformationMatrix;
}

std::vector<float> View::getZoomedMapTransformationMatrix(
    std::pair<double, double> physicalMazeSize,
    std::pair<int, int> zoomedMapPosition,
    std::pair<int, int> zoomedMapSize,
    const Coordinate& initialMouseTranslation,
    const Coordinate& currentMouseTranslation,
    const Angle& currentMouseRotation) {

    // The purpose of this function is to produce a 4x4 matrix which,
    // when applied to the physical coordinate within the vertex shader,
    // transforms it into an OpenGL coordinate for the zoomed map. Note that
    // the zoomed map will likely not contain the entirety of the maze, so the
    // pixel coordinates will be outside of the map.

    // Step 1: Calculate the scaling matrix
    double physicalWidth = physicalMazeSize.first;
    double physicalHeight = physicalMazeSize.second;

    // Note that this is not literally the number of pixels per meter of the
    // screen. Rather, it's our desired number of pixels per simulation meter.
    double pixelsPerMeter = getScreenPixelsPerMeter() * S()->zoomedMapScale();
    double pixelWidth = pixelsPerMeter * physicalWidth;
    double pixelHeight = pixelsPerMeter * physicalHeight;

    std::pair<double, double> openGlOrigin = mapPixelCoordinateToOpenGlCoordinate(0, 0, getWindowSize().first, getWindowSize().second);
    std::pair<double, double> openGlMazeSize = mapPixelCoordinateToOpenGlCoordinate(pixelWidth, pixelHeight, getWindowSize().first, getWindowSize().second);
    double openGlWidth = openGlMazeSize.first - openGlOrigin.first;
    double openGlHeight = openGlMazeSize.second - openGlOrigin.second;

    double horizontalScaling = openGlWidth / physicalWidth;
    double verticalScaling = openGlHeight / physicalHeight;

    std::vector<float> scalingMatrix = {
        static_cast<float>(horizontalScaling),                                 0.0, 0.0, 0.0,
                                          0.0, static_cast<float>(verticalScaling), 0.0, 0.0,
                                          0.0,                                 0.0, 1.0, 0.0,
                                          0.0,                                 0.0, 0.0, 1.0,
    };

    // Step 2: Construct the translation matrix. We must ensure that the mouse
    // starts (static translation) and stays (dynamic translation) at the
    // center of the screen.

    // Part A: Find the static translation, i.e., the translation that puts the
    // center of the mouse (i.e., the midpoint of the line connecting its
    // wheels) in the center of the zoomed map. 
    double centerXPixels = initialMouseTranslation.getX().getMeters() * pixelsPerMeter;
    double centerYPixels = initialMouseTranslation.getY().getMeters() * pixelsPerMeter;
    double zoomedMapCenterXPixels = zoomedMapPosition.first + 0.5 * zoomedMapSize.first;
    double zoomedMapCenterYPixels = zoomedMapPosition.second + 0.5 * zoomedMapSize.second;
    double staticTranslationXPixels = zoomedMapCenterXPixels - centerXPixels;
    double staticTranslationYPixels = zoomedMapCenterYPixels - centerYPixels;
    std::pair<double, double> staticTranslation =
        mapPixelCoordinateToOpenGlCoordinate(staticTranslationXPixels, staticTranslationYPixels, getWindowSize().first, getWindowSize().second);

    // Part B: Find the dynamic translation, i.e., the current translation of the mouse.
    Cartesian mouseTranslationDelta = Cartesian(currentMouseTranslation) - initialMouseTranslation;
    double dynamicTranslationXPixels = mouseTranslationDelta.getX().getMeters() * pixelsPerMeter;
    double dynamicTranslationYPixels = mouseTranslationDelta.getY().getMeters() * pixelsPerMeter;
    std::pair<double, double> dynamicTranslation =
        mapPixelCoordinateToOpenGlCoordinate(dynamicTranslationXPixels, dynamicTranslationYPixels, getWindowSize().first, getWindowSize().second);

    // Combine the transalations and form the translation matrix
    double horizontalTranslation = staticTranslation.first -  dynamicTranslation.first + openGlOrigin.first;
    double verticalTranslation = staticTranslation.second - dynamicTranslation.second + openGlOrigin.second;
    std::vector<float> translationMatrix = {
        1.0, 0.0, 0.0, static_cast<float>(horizontalTranslation), 
        0.0, 1.0, 0.0,   static_cast<float>(verticalTranslation),
        0.0, 0.0, 1.0,                                       0.0,
        0.0, 0.0, 0.0,                                       1.0,
    };

    // Step 3: Construct a few other transformation matrices needed for
    // rotating the maze. In order to properly rotate the maze, we must first
    // translate the center of the mouse to the origin. Then we have to unscale
    // it, rotate it, scale it, and then translate it back to the proper
    // location. Hence all of the matrices.

    // We subtract Degrees(90) here since we want forward to face NORTH
    double theta = (Degrees(currentMouseRotation) - Degrees(90)).getRadiansZeroTo2pi();
    std::vector<float> rotationMatrix = {
        static_cast<float>( std::cos(theta)), static_cast<float>(std::sin(theta)), 0.0, 0.0,
        static_cast<float>(-std::sin(theta)), static_cast<float>(std::cos(theta)), 0.0, 0.0,
                                    0.0,                            0.0, 1.0, 0.0,
                                    0.0,                            0.0, 0.0, 1.0,
    };

    std::vector<float> inverseScalingMatrix = {
        static_cast<float>(1.0/horizontalScaling),                                     0.0, 0.0, 0.0,
                                              0.0, static_cast<float>(1.0/verticalScaling), 0.0, 0.0,
                                              0.0,                                     0.0, 1.0, 0.0,
                                              0.0,                                     0.0, 0.0, 1.0,
    };

    std::pair<double, double> zoomedMapCenterOpenGl =
        mapPixelCoordinateToOpenGlCoordinate(zoomedMapCenterXPixels, zoomedMapCenterYPixels, getWindowSize().first, getWindowSize().second);
    std::vector<float> translateToOriginMatrix = {
        1.0, 0.0, 0.0,  static_cast<float>(zoomedMapCenterOpenGl.first),
        0.0, 1.0, 0.0, static_cast<float>(zoomedMapCenterOpenGl.second),
        0.0, 0.0, 1.0,                                              0.0,
        0.0, 0.0, 0.0,                                              1.0,
    };

    std::vector<float> inverseTranslateToOriginMatrix = {
        1.0, 0.0, 0.0,  static_cast<float>(-zoomedMapCenterOpenGl.first),
        0.0, 1.0, 0.0, static_cast<float>(-zoomedMapCenterOpenGl.second),
        0.0, 0.0, 1.0,                                               0.0,
        0.0, 0.0, 0.0,                                               1.0,
    };

    std::vector<float> zoomedMapCameraMatrix = multiply4x4Matrices(translationMatrix, scalingMatrix);
    if (S()->rotateZoomedMap()) {
        zoomedMapCameraMatrix =
            multiply4x4Matrices(translateToOriginMatrix,
            multiply4x4Matrices(scalingMatrix,
            multiply4x4Matrices(rotationMatrix,
            multiply4x4Matrices(inverseScalingMatrix,
            multiply4x4Matrices(inverseTranslateToOriginMatrix,
                                zoomedMapCameraMatrix)))));
    }

    return zoomedMapCameraMatrix;
}

std::vector<float> View::multiply4x4Matrices(std::vector<float> left, std::vector<float> right) {
    ASSERT_EQ(left.size(), 16);
    ASSERT_EQ(right.size(), 16);
    std::vector<float> result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double value = 0.0;
            for (int k = 0; k < 4; k++) {
                value += left.at(4*i+k) * right.at(4*k+j);
            }
            result.push_back(value);
        }
    }
    ASSERT_EQ(result.size(), 16);
    return result;
}

double View::getScreenPixelsPerMeter() {
    // TODO: MACK - make this part of initialization
    // Assumptions:
    // 1) These values will not change during an execution of the program
    // 2) Monitor pixels are square (and thus the pixels per meter is the same
    //    for both the horizontal and vertical direcitons).
    static double pixels = glutGet(GLUT_SCREEN_WIDTH);
    static double millimeters = glutGet(GLUT_SCREEN_WIDTH_MM);
    static double pixelsPerMeter = 1000 * pixels / millimeters;
    return pixelsPerMeter;
}

void View::insertIntoGraphicCpuBuffer(const Polygon& polygon, Color color, double alpha) {
    std::vector<TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, alpha);
    for (int i = 0; i < tgs.size(); i += 1) {
        GRAPHIC_CPU_BUFFER.push_back(tgs.at(i));
    }
}

void View::insertIntoTextureCpuBuffer() {
    // Here we just insert dummy TriangleTexture objects. All of the actual
    // values of the objects will be set on calls to the update method.
    TriangleTexture t {
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0},
    };
    // There are exactly two triangle texture objects needed, since there are
    // two triangles per rectangular texture
    for (int i = 0; i < 2; i += 1) {
        TEXTURE_CPU_BUFFER.push_back(t);
    }
}

std::pair<int, int> View::getTileGraphicTextMaxSize() {
    static std::pair<int, int> maxRowsAndCols = std::make_pair(2, 4);
    return maxRowsAndCols;
}

void View::updateTileGraphicBaseColor(int x, int y, Color color) {
    int index = getTileGraphicBaseStartingIndex(x, y);
    std::tuple<double, double, double> colorValues = COLOR_TO_RGB.at(color);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &GRAPHIC_CPU_BUFFER.at(index + i);
        triangleGraphic->p1.r = std::get<0>(colorValues);
        triangleGraphic->p1.g = std::get<1>(colorValues);
        triangleGraphic->p1.b = std::get<2>(colorValues);
        triangleGraphic->p2.r = std::get<0>(colorValues);
        triangleGraphic->p2.g = std::get<1>(colorValues);
        triangleGraphic->p2.b = std::get<2>(colorValues);
        triangleGraphic->p3.r = std::get<0>(colorValues);
        triangleGraphic->p3.g = std::get<1>(colorValues);
        triangleGraphic->p3.b = std::get<2>(colorValues);
    }
}

void View::updateTileGraphicWallColor(int x, int y, Direction direction, Color color, double alpha) {
    int index = getTileGraphicWallStartingIndex(x, y, direction);
    std::tuple<double, double, double> colorValues = COLOR_TO_RGB.at(color);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &GRAPHIC_CPU_BUFFER.at(index + i);
        triangleGraphic->p1.r = std::get<0>(colorValues);
        triangleGraphic->p1.g = std::get<1>(colorValues);
        triangleGraphic->p1.b = std::get<2>(colorValues);
        triangleGraphic->p1.a = alpha;
        triangleGraphic->p2.r = std::get<0>(colorValues);
        triangleGraphic->p2.g = std::get<1>(colorValues);
        triangleGraphic->p2.b = std::get<2>(colorValues);
        triangleGraphic->p2.a = alpha;
        triangleGraphic->p3.r = std::get<0>(colorValues);
        triangleGraphic->p3.g = std::get<1>(colorValues);
        triangleGraphic->p3.b = std::get<2>(colorValues);
        triangleGraphic->p3.a = alpha;
    }
}

void View::updateTileGraphicFog(int x, int y, double alpha) {
    int index = getTileGraphicFogStartingIndex(x, y);
    for (int i = 0; i < 2; i += 1) {
        TriangleGraphic* triangleGraphic = &GRAPHIC_CPU_BUFFER.at(index + i);
        triangleGraphic->p1.a = alpha;
        triangleGraphic->p2.a = alpha;
        triangleGraphic->p3.a = alpha;
    }
}

void View::updateTileGraphicText(const Tile* tile, int numRows, int numCols, int row, int col, char c) {

    //                          col
    //
    //             *---*-------------------*---*
    //             |   |                   |   |
    //             *---*------------------[Y]--*
    //             |   |    |    |    |    |   |
    //             |   |    |    |    |    |   |
    //             |   | 00 | 01 | 02 | 03 |   |
    // rowsFromTop |   |____|____|____|____|   |
    //             |   |    |    |    |    |   |
    //             |   |    |    |    |    |   |
    //             |   | 10 | 11 | 12 | 13 |   |
    //             |   |    |    |    |    |   |
    //             *--[X]------------------*---*
    //             |   |                   |   |
    //             *---*-------------------*---*

    std::pair<int, int> maxRowsAndCols = View::getTileGraphicTextMaxSize();
    Cartesian X = tile->getInteriorPolygon().getVertices().at(0);
    Cartesian Y = tile->getInteriorPolygon().getVertices().at(2);
    Cartesian diagonal = Y - X;
    Meters characterWidth = diagonal.getX() / static_cast<double>(maxRowsAndCols.second);
    Meters characterHeight = diagonal.getY() / static_cast<double>(maxRowsAndCols.first);

    // The rows start from the top and increase down the cell
    int rowsFromTop = maxRowsAndCols.first - row - 1;
    double rowOffset = rowsFromTop - static_cast<double>(maxRowsAndCols.first - numRows) / 2.0;
    double colOffset = col + static_cast<double>(maxRowsAndCols.second - numCols) / 2.0;

    int fontImageCharacterIndex = FONT_IMAGE_CHARS.find_first_of(c);
    VertexTexture p1 = { // LL
        (X + Cartesian(characterWidth *  colOffset     , characterHeight *  rowOffset     )).getX().getMeters(),
        (X + Cartesian(characterWidth *  colOffset     , characterHeight *  rowOffset     )).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(FONT_IMAGE_CHARS.size()),
        0.0,
    };
    VertexTexture p2 = { // UL
        (X + Cartesian(characterWidth *  colOffset     , characterHeight * (rowOffset + 1))).getX().getMeters(),
        (X + Cartesian(characterWidth *  colOffset     , characterHeight * (rowOffset + 1))).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex) / static_cast<double>(FONT_IMAGE_CHARS.size()),
        1.0,
    };
    VertexTexture p3 = { // UR
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight * (rowOffset + 1))).getX().getMeters(),
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight * (rowOffset + 1))).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(FONT_IMAGE_CHARS.size()),
        1.0,
    };
    VertexTexture p4 = { // LR
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight *  rowOffset     )).getX().getMeters(),
        (X + Cartesian(characterWidth * (colOffset + 1), characterHeight *  rowOffset     )).getY().getMeters(),
        static_cast<double>(fontImageCharacterIndex + 1) / static_cast<double>(FONT_IMAGE_CHARS.size()),
        0.0,
    };

    int triangleTextureIndex = getTileGraphicTextStartingIndex(tile->getX(), tile->getY(), row, col);
    TEXTURE_CPU_BUFFER.at(triangleTextureIndex) = {p1, p2, p3};
    TEXTURE_CPU_BUFFER.at(triangleTextureIndex + 1) = {p1, p3, p4};
}

void View::drawMousePolygon(const Polygon& polygon, Color color, double sensorAlpha) {
    std::vector<TriangleGraphic> tgs = polygonToTriangleGraphics(polygon, color, sensorAlpha);
    GRAPHIC_CPU_BUFFER.insert(GRAPHIC_CPU_BUFFER.end(), tgs.begin(), tgs.end());
}

std::vector<TriangleGraphic> View::polygonToTriangleGraphics(const Polygon& polygon, Color color, double alpha) {
    std::vector<Triangle> triangles = polygon.getTriangles();
    std::vector<TriangleGraphic> triangleGraphics;
    std::tuple<double, double, double> colorValues = COLOR_TO_RGB.at(color);
    for (Triangle triangle : triangles) {
        triangleGraphics.push_back({
            {triangle.p1.getX().getMeters(), triangle.p1.getY().getMeters(),
                std::get<0>(colorValues), std::get<1>(colorValues), std::get<2>(colorValues), alpha},
            {triangle.p2.getX().getMeters(), triangle.p2.getY().getMeters(),
                std::get<0>(colorValues), std::get<1>(colorValues), std::get<2>(colorValues), alpha},
            {triangle.p3.getX().getMeters(), triangle.p3.getY().getMeters(),
                std::get<0>(colorValues), std::get<1>(colorValues), std::get<2>(colorValues), alpha}});
    }
    return triangleGraphics;
}

int View::trianglesPerTile() {
    // This value must be predetermined, and was done so as follows:
    // Base polygon:      2 (2 triangles x 1 polygon  per tile)
    // Wall polygon:      8 (2 triangles x 4 polygons per tile)
    // Corner polygon:    8 (2 triangles x 4 polygons per tile)
    // Fog polygon:       2 (2 triangles x 1 polygon  per tile)
    // --------------------
    // Total             20
    return 20;
}

int View::getTileGraphicBaseStartingIndex(int x, int y) {
    return  0 + trianglesPerTile() * (m_model->getMaze()->getHeight() * x + y);
}

int View::getTileGraphicWallStartingIndex(int x, int y, Direction direction) {
    return  2 + trianglesPerTile() * (m_model->getMaze()->getHeight() * x + y) + (2 * SimUtilities::getDirectionIndex(direction));
}

int View::getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber) {
    return 10 + trianglesPerTile() * (m_model->getMaze()->getHeight() * x + y) + (2 * cornerNumber);
}

int View::getTileGraphicFogStartingIndex(int x, int y) {
    return 18 + trianglesPerTile() * (m_model->getMaze()->getHeight() * x + y);
}

int View::getTileGraphicTextStartingIndex(int x, int y, int row, int col) {
    static std::pair<int, int> maxRowsAndCols = getTileGraphicTextMaxSize();
    static int triangleTexturesPerTile = 2 * maxRowsAndCols.first * maxRowsAndCols.second;
    return triangleTexturesPerTile * (m_model->getMaze()->getHeight() * x + y) + 2 * (row * maxRowsAndCols.second + col);
}

} // namespace sim
