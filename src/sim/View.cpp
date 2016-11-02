#include "View.h"

#include <QDebug>
#include <QFile>
#include <QPair>

#include "BufferInterface.h"
#include "Controller.h"
#include "Directory.h"
#include "Layout.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "TransformationMatrix.h"
#include "units/Seconds.h"

namespace mms {

View::View(
        Model* model,
        int argc,
        char* argv[],
        QWidget* parent) :
        QOpenGLWidget(parent),
        m_model(model) {

    m_bufferInterface = new BufferInterface(
        {m_model->getMaze()->getWidth(), m_model->getMaze()->getHeight()},
        &m_graphicCpuBuffer,
        &m_textureCpuBuffer
    );
    m_mazeGraphic = new MazeGraphic(model->getMaze(), m_bufferInterface);
    m_mouseGraphic = new MouseGraphic(model->getMouse(), m_bufferInterface);
    m_fontImageMap = getFontImageMap();
    m_header = new Header(model);
}

MazeGraphic* View::getMazeGraphic() {
    return m_mazeGraphic;
}

MouseGraphic* View::getMouseGraphic() {
    return m_mouseGraphic;
}

void View::setController(Controller* controller) {
    m_controller = controller;
    m_header->setController(controller);
}

void View::refresh() {

    // In order to ensure we're sleeping the correct amount of time, we time
    // the drawing operation and take it into account when we sleep.
    double start(SimUtilities::getHighResTimestamp());

    // TODO: MACK - this shouldn't be here :/
    // First, clear fog as necessary
    if (m_controller->getDynamicOptions().automaticallyClearFog) {
        // TODO: upforgrabs
        // This won't work if the mouse is traveling too quickly and travels more
        // than one tile per frame. Figure out a way that will work in that case.
        QPair<int, int> currentPosition =
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

    // Re-populate both vertex buffer objects
    repopulateVertexBufferObjects();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable scissoring so that the maps are only draw in specified locations.
    glEnable(GL_SCISSOR_TEST);

    ////////////
	// m_polygonProgram.bind();
	// m_polygonVAO.bind();
	// glDrawArrays(GL_TRIANGLES, 0, 3);
	// m_polygonVAO.release();
	// m_polygonProgram.release();
    ////////////

    // Draw the tiles
    drawFullAndZoomedMaps(
        currentMouseTranslation,
        currentMouseRotation,
        &m_polygonProgram,
        &m_polygonVAO,
        0,
        3 * mouseTrianglesStartingIndex
    );

    // Draw the tile text
    // TODO: MACK
    // drawFullAndZoomedMaps(currentMouseTranslation, currentMouseRotation,
    //     m_textureProgram, m_textureVertexArrayObjectId, 0, 3 * m_textureCpuBuffer.size());
    // drawFullAndZoomedMaps(currentMouseTranslation, currentMouseRotation,
    //     m_textureVertexArrayObjectId, 0, 3 * m_textureCpuBuffer.size());

    // Draw the mouse
    // // TODO: MACK
    // // drawFullAndZoomedMaps(currentMouseTranslation, currentMouseRotation,
    // //     m_polygonProgram, m_polygonVertexArrayObjectId, 3 * mouseTrianglesStartingIndex,
    // //     3 * (m_graphicCpuBuffer.size() - mouseTrianglesStartingIndex));
    // drawFullAndZoomedMaps(currentMouseTranslation, currentMouseRotation,
    //     m_polygonVertexArrayObjectId, 3 * mouseTrianglesStartingIndex,
    //     3 * (m_graphicCpuBuffer.size() - mouseTrianglesStartingIndex));

    // Disable scissoring so that the glClear can take effect, and so that
    // drawn text isn't clipped at all
    glDisable(GL_SCISSOR_TEST);

    // Draw the window header
    // TODO: MACK
    // m_header->draw();

    // TODO: MACK
    // Display the result
    // glSwapBuffers();

    // Get the duration of the drawing operation, in seconds. Note that this duration
    // is simply the total number of real seconds that have passed, which is exactly
    // what we want (since the frame-rate is perceived in real-time and not CPU time).
    double end(SimUtilities::getHighResTimestamp());
    double duration = end - start;

    // Notify the user of a late frame
    // TODO: MACK - make variables for these long expressions
    if (P()->printLateFrames() && duration > 1.0/P()->frameRate()) {
        // TODO: MACK
        // qWarning().noquote().nospace()
        //     << "A frame was late by " << duration - 1.0/P()->frameRate()
        //     << " seconds, which is "
        //     << (duration - 1.0/P()->frameRate())/(1.0/P()->frameRate()) * 100
        //     << " percent late.";
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

QSet<QChar> View::getAllowableTileTextCharacters() {
    return QSet<QChar>::fromList(m_fontImageMap.keys());
}

void View::initTileGraphicText() {

    // Initialze the tile text in the buffer class, do caching for speed improvement
    m_bufferInterface->initTileGraphicText(
        Meters(P()->wallLength()),
        Meters(P()->wallWidth()),
        {
            m_controller->getStaticOptions().tileTextNumberOfRows,
            m_controller->getStaticOptions().tileTextNumberOfCols
        },
        m_fontImageMap,
        P()->tileTextBorderFraction(),
        STRING_TO_TILE_TEXT_ALIGNMENT.value(P()->tileTextAlignment()));
}

void View::keyPress(unsigned char key, int x, int y) {

    // NOTE: If you're adding or removing anything from this function, make
    // sure to update wiki/Keys.md

    if (key == 'p') {
        // Toggle pause (only in discrete mode)
        if (
            STRING_TO_INTERFACE_TYPE.value(m_controller->getStaticOptions().interfaceType)
            == InterfaceType::DISCRETE
        ) {
            S()->setPaused(!S()->paused());
        }
        else {
            qWarning().noquote().nospace()
                << "Pausing the simulator is only allowed in "
                << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
                << " mode.";
        }
    }
    else if (key == 'f') {
        // Faster (only in discrete mode)
        if (
            STRING_TO_INTERFACE_TYPE.value(m_controller->getStaticOptions().interfaceType)
            == InterfaceType::DISCRETE
        ) {
            S()->setSimSpeed(S()->simSpeed() * 1.5);
        }
        else {
            qWarning().noquote().nospace()
                << "Increasing the simulator speed is only allowed in "
                << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
                << " mode.";
        }
    }
    else if (key == 's') {
        // Slower (only in discrete mode)
        if (
            STRING_TO_INTERFACE_TYPE.value(m_controller->getStaticOptions().interfaceType)
            == InterfaceType::DISCRETE
        ) {
            S()->setSimSpeed(S()->simSpeed() / 1.5);
        }
        else {
            qWarning().noquote().nospace()
                << "Decreasing the simulator speed is only allowed in "
                << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
                << " mode.";
        }
    }
    else if (key == 'l') {
        // Cycle through the available layouts
        S()->setLayoutType(LAYOUT_TYPE_CYCLE.value(S()->layoutType()));
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
    else if (QString("0123456789").indexOf(key) != -1) {
        // Press an input button
        int inputButton = QString("0123456789").indexOf(key);
        if (!S()->inputButtonWasPressed(inputButton)) {
            S()->setInputButtonWasPressed(inputButton, true);
            qInfo().noquote().nospace()
                << "Input button " << inputButton << " was pressed.";
        }
        else {
            qWarning().noquote().nospace()
                << "Input button " << inputButton << " has not yet been"
                << " acknowledged as pressed; pressing it has no effect.";
        }
    }
}

void View::specialKeyPress(int key, int x, int y) {
    if (!INT_TO_KEY.contains(key)) {
        return;
    }
    if (ARROW_KEYS.contains(INT_TO_KEY.value(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.value(key), true);
    }
}

void View::specialKeyRelease(int key, int x, int y) {
    if (!INT_TO_KEY.contains(key)) {
        return;
    }
    if (ARROW_KEYS.contains(INT_TO_KEY.value(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.value(key), false);
    }
}

void View::initializeGL() {

    // TODO: MACK
    initializeOpenGLFunctions();
    printVersionInformation();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, S()->wireframeMode() ? GL_LINE : GL_FILL);

    initLogger();

    // TODO: MACK
    initPolygonProgram();
    // initTextureProgram();
    // TODO: MACK - fix this
    m_screenPixelsPerMeter = 10000; // glutGet(GLUT_SCREEN_WIDTH) / (glutGet(GLUT_SCREEN_WIDTH_MM) / 1000.0);

	connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void View::onMessageLogged(QOpenGLDebugMessage message) {
    qDebug() << message;
}

void View::resizeGL(int w, int h) {
    // TODO: MACK
    updateWindowSize(w, h);
} 

void View::paintGL() {
    refresh();
} 


void View::initGraphics(int argc, char* argv[]) {

    // GLUT Initialization
    //glutInit(&argc, argv); // TODO: MACK

    // XXX: Is this necessary on OSX?
    // glutInitDisplayMode(2048 | GLUT_DOUBLE | GLUT_RGBA);
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    // glutInitWindowPosition(0, 0);
    // glutInitWindowSize(P()->defaultWindowWidth(), P()->defaultWindowHeight());
    // glutCreateWindow("Micromouse Simulator");

    // glClearColor(0.0, 0.0, 0.0, 1.0);

    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_BLEND);
    // glPolygonMode(GL_FRONT_AND_BACK, S()->wireframeMode() ? GL_LINE : GL_FILL);

    // glutDisplayFunc(functions.refresh);
    // glutIdleFunc(functions.refresh);
    // glutReshapeFunc(functions.windowResize);
    // glutKeyboardFunc(functions.keyPress);
    // glutSpecialFunc(functions.specialKeyPress);
    // glutSpecialUpFunc(functions.specialKeyRelease);

    // XXX: If this necessary on OSX?
    // glewExperimental = GL_TRUE;

    // GLEW Initialization
    // GLenum err = glewInit();
    // if (GLEW_OK != err) {
    //     qCritical().noquote().nospace() << "Unable to initialize GLEW.";
    //     SimUtilities::quit();
    // }

    // XXX: Print out the OpenGL version
    // std::cout << glGetString(GL_VERSION) << std::endl;
}

void View::initPolygonProgram() {

	m_polygonProgram.addShaderFromSourceCode(
		QOpenGLShader::Vertex,
		R"(
            uniform mat4 transformationMatrix;
            attribute vec2 coordinate;
            attribute vec4 inColor;
            varying vec4 outColor;
            void main(void) {
                gl_Position = transformationMatrix * vec4(coordinate, 0.0, 1.0);
                outColor = inColor;
            }
		)"
	);
	m_polygonProgram.addShaderFromSourceCode(
		QOpenGLShader::Fragment,
		R"(
            varying vec4 outColor;
			void main(void) {
			   gl_FragColor = outColor;
			}
		)"
	);
	m_polygonProgram.link();
	m_polygonProgram.bind();

	m_polygonVAO.create();
	m_polygonVAO.bind();

	m_polygonVBO.create();
	m_polygonVBO.bind();
	m_polygonVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);

	m_polygonProgram.enableAttributeArray("coordinate");
	m_polygonProgram.setAttributeBuffer(
        "coordinate", // name
        GL_DOUBLE, // type
        0, // offset (bytes)
        2, // tupleSize (number of elements in the attribute array)
        6 * sizeof(double) // stride (bytes between vertices)
    );

	m_polygonProgram.enableAttributeArray("inColor");
	m_polygonProgram.setAttributeBuffer(
        "inColor", // name
        GL_DOUBLE, // type
        2 * sizeof(double), // offset (bytes)
        4, // tupleSize (number of elements in the attribute array)
        6 * sizeof(double) // stride (bytes between vertices)
    );

	m_polygonVBO.release();
	m_polygonVAO.release();
	m_polygonProgram.release();

    // TODO: MACK
    /*
    // Generate the polygon vertex array object and vertex buffer object
    glGenVertexArrays(1, &m_polygonVertexArrayObjectId);
    glBindVertexArray(m_polygonVertexArrayObjectId);
    glGenBuffers(1, &m_polygonVertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, m_polygonVertexBufferObjectId);

    // Set up the program and attribute pointers
    m_polygonProgram = new tdogl::Program({tdogl::Shader::shaderFromFile(
        Directory::get()->getResShadersDirectory().toStdString() +
        "polygonVertexShader.txt", GL_VERTEX_SHADER)});

    glEnableVertexAttribArray(m_polygonProgram.attributeLocation("coordinate"));
    glVertexAttribPointer(
        m_polygonProgram.attributeLocation("coordinate"),
        2, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), 0
    );

    glEnableVertexAttribArray(m_polygonProgram.attributeLocation("color"));
    glVertexAttribPointer(
        m_polygonProgram.attributeLocation("color"),
        4, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (char*) NULL + 2 * sizeof(double)
    );

    // Unbind the vertex array object and vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    */
}

void View::initTextureProgram() {

    // TODO: MACK
    /*
    // Generate the texture vertex array object and vertex buffer object
    glGenVertexArrays(1, &m_textureVertexArrayObjectId);
    glBindVertexArray(m_textureVertexArrayObjectId);
    glGenBuffers(1, &m_textureVertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureVertexBufferObjectId);

    Set up the program and attribute pointers
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(
        Directory::get()->getResShadersDirectory().toStdString() + "textureVertexShader.txt", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(
        Directory::get()->getResShadersDirectory().toStdString() + "textureFragmentShader.txt", GL_FRAGMENT_SHADER));
    m_textureProgram = new tdogl::Program(shaders);
    glEnableVertexAttribArray(m_textureProgram->attrib("coordinate"));
    glVertexAttribPointer(m_textureProgram->attrib("coordinate"),
        2, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), NULL);
    glEnableVertexAttribArray(m_textureProgram->attrib("textureCoordinate"));
    glVertexAttribPointer(m_textureProgram->attrib("textureCoordinate"),
        2, GL_DOUBLE, GL_TRUE, 4 * sizeof(double), (char*) NULL + 2 * sizeof(double));

    Load the bitmap texture into the texture atlas
    QString tileTextFontImagePath = Directory::get()->getResImgsDirectory() + P()->tileTextFontImage();
    if (!QFile::exists(tileTextFontImagePath)) {
        qCritical().noquote().nospace()
            << "Could not find font image file \"" << P()->tileTextFontImage()
            << "\" in \"" << Directory::get()->getResImgsDirectory() << "\".";
        SimUtilities::quit();
    }
    tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(tileTextFontImagePath.toStdString());
    bmp.flipVertically();
    m_textureAtlas = new tdogl::Texture(bmp);

    // Unbind the vertex array object and vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    */
}

QMap<QChar, QPair<double, double>> View::getFontImageMap() {

    // These values must perfectly reflect the font image being used, or else
    // the wrong characters will be displayed on the tiles.
    QString fontImageChars =
        " !\"#$%&'()*+,-./0123456789:;<=>?"
        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
        "`abcdefghijklmnopqrstuvwxyz{|}~";

    // Get a map of the font image characters (allowable tile text characters)
    // to their position in the png image (a fraction from 0.0 to 1.0)
    QMap<QChar, QPair<double, double>> fontImageMap;
    for (int i = 0; i < fontImageChars.size(); i += 1) {
        fontImageMap.insert(
            fontImageChars.at(i),
            {
                static_cast<double>(i + 0) / static_cast<double>(fontImageChars.size()),
                static_cast<double>(i + 1) / static_cast<double>(fontImageChars.size())
            }
        );
    }

    // Return the map
    return fontImageMap;
}

void View::repopulateVertexBufferObjects() {

    m_polygonVBO.bind();
	m_polygonVBO.allocate(
        &m_graphicCpuBuffer.front(),
        m_graphicCpuBuffer.size() * sizeof(double)
    );
    // TODO: MACK - necessary?
    // m_polygonVBO.write(
    //     0,
    //     &m_graphicCpuBuffer.front(),
    //     m_graphicCpuBuffer.size() * sizeof(double)
    // );
    m_polygonVBO.release();

    // TODO: MACK
    /*
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
    */
}

void View::drawFullAndZoomedMaps(
        const Coordinate& currentMouseTranslation,
        const Angle& currentMouseRotation,
        QOpenGLShaderProgram* program,
        QOpenGLVertexArrayObject* vao,
        int vboStartingIndex,
        int vboEndingIndex) {

    // Get the sizes and positions of each of the maps.
    QPair<int, int> fullMapPosition = Layout::getFullMapPosition(
        m_windowWidth, m_windowHeight, m_header->getHeight(), P()->windowBorderWidth(), S()->layoutType());
    QPair<int, int> fullMapSize = Layout::getFullMapSize(
        m_windowWidth, m_windowHeight, m_header->getHeight(), P()->windowBorderWidth(), S()->layoutType());
    QPair<int, int> zoomedMapPosition = Layout::getZoomedMapPosition(
        m_windowWidth, m_windowHeight, m_header->getHeight(), P()->windowBorderWidth(), S()->layoutType());
    QPair<int, int> zoomedMapSize = Layout::getZoomedMapSize(
        m_windowWidth, m_windowHeight, m_header->getHeight(), P()->windowBorderWidth(), S()->layoutType());

    // Get the physical size of the maze (in meters)
    double physicalMazeWidth = P()->wallWidth() + m_model->getMaze()->getWidth() * (P()->wallWidth() + P()->wallLength());
    double physicalMazeHeight = P()->wallWidth() + m_model->getMaze()->getHeight() * (P()->wallWidth() + P()->wallLength());
    // TODO: MACK - these should be distances, not doubles
    QPair<double, double> physicalMazeSize = {physicalMazeWidth, physicalMazeHeight};

    // Start using the program and vertex array object
    program->bind();
    vao->bind();

    // If it's the texture program, bind the texture and set the uniform
    // TODO: MACK
    /*
    if (program == m_textureProgram) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureAtlas->object());
        program->setUniform("_texture", 0);
    }
    */

    // Render the full map
    // TODO: MACK
    auto matrix = TransformationMatrix::getFullMapTransformationMatrix(
        Meters(P()->wallWidth()),
        physicalMazeSize,
        fullMapPosition,
        fullMapSize,
        {m_windowWidth, m_windowHeight}
    );
    QMatrix4x4 transformationMatrix(
        matrix.at(0), matrix.at(1), matrix.at(2), matrix.at(3),
        matrix.at(4), matrix.at(5), matrix.at(6), matrix.at(7),
        matrix.at(8), matrix.at(9), matrix.at(10), matrix.at(11),
        matrix.at(12), matrix.at(13), matrix.at(14), matrix.at(15)
    );
    glScissor(fullMapPosition.first, fullMapPosition.second, fullMapSize.first, fullMapSize.second);
    program->setUniformValue("transformationMatrix", transformationMatrix);
    glDrawArrays(GL_TRIANGLES, vboStartingIndex, vboEndingIndex);

    // Render the zoomed map
    // TODO: MACK
    /*
    glScissor(zoomedMapPosition.first, zoomedMapPosition.second, zoomedMapSize.first, zoomedMapSize.second);
    program->setUniformMatrix4("transformationMatrix",
        &TransformationMatrix::getZoomedMapTransformationMatrix(
            physicalMazeSize,
            zoomedMapPosition,
            zoomedMapSize,
            {m_windowWidth, m_windowHeight},
            m_screenPixelsPerMeter,
            S()->zoomedMapScale(),
            S()->rotateZoomedMap(),
            m_model->getMouse()->getInitialTranslation(),
            currentMouseTranslation,
            currentMouseRotation).front(), 1, GL_TRUE);
    glDrawArrays(GL_TRIANGLES, vboStartingIndex, vboEndingIndex);
    */

    // Stop using the program and vertex array object
    vao->release();
    program->release();

    // If it's the texture program, we should additionally unbind the texture
    // TODO: MACK
    /*
    if (program == m_textureProgram) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    */
}

void View::initLogger() {
    connect(
        &m_logger,
        SIGNAL(messageLogged(QOpenGLDebugMessage)),
        this,
        SLOT(onMessageLogged(QOpenGLDebugMessage))
    );
    if (m_logger.initialize()) {
        // TODO: MACK - change to asynchronous
        m_logger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
        m_logger.enableMessages();
        if (true) {
            m_logger.disableMessages(
                QOpenGLDebugMessage::AnySource,
                QOpenGLDebugMessage::AnyType,
                QOpenGLDebugMessage::NotificationSeverity
            );
        }
    }
}

void View::printVersionInformation() {

  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

  // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile()) {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";

}

} // namespace mms
