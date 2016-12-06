#include "View.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QPair>

#include "BufferInterface.h"
#include "ControllerManager.h"
#include "Directory.h"
#include "Layout.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "Screen.h"
#include "State.h"
#include "TransformationMatrix.h"
#include "units/Seconds.h"

namespace mms {

View::View(Model* model, QWidget* parent) :
        QOpenGLWidget(parent),
        m_model(model) {

    // Initialize the buffer and graphics objects
    m_bufferInterface = new BufferInterface(
        {m_model->getMaze()->getWidth(), m_model->getMaze()->getHeight()},
        &m_graphicCpuBuffer,
        &m_textureCpuBuffer
    );
    m_mazeGraphic = new MazeGraphic(model->getMaze(), m_bufferInterface);
    m_mouseGraphic = new MouseGraphic(model->getMouse(), m_bufferInterface);

    m_fontImageMap = getFontImageMap();
    m_header = new Header(model);

    // Ensure that we continuously refresh the widget
	connect(
        this, &View::frameSwapped,
        this, static_cast<void (View::*)()>(&View::update)
    );

    // TODO: MACK
    m_mazeGraphic->drawPolygons();

    // TODO: MACK - get these defaults from somewhere
    initTileGraphicText(2, 4);
}

MazeGraphic* View::getMazeGraphic() {
    return m_mazeGraphic;
}

MouseGraphic* View::getMouseGraphic() {
    return m_mouseGraphic;
}

QSet<QChar> View::getAllowableTileTextCharacters() {
    return QSet<QChar>::fromList(m_fontImageMap.keys());
}

void View::initTileGraphicText(int numRows, int numCols) {

    // Initialze the tile text in the buffer class, do caching for speed improvement
    m_bufferInterface->initTileGraphicText(
        Meters(P()->wallLength()),
        Meters(P()->wallWidth()),
        {numRows, numCols},
        m_fontImageMap,
        P()->tileTextBorderFraction(),
        STRING_TO_TILE_TEXT_ALIGNMENT.value(P()->tileTextAlignment()));

    // TODO: MACK - this is kind of confusing
    // - I should insert and then update (no draw method)
    m_textureCpuBuffer.clear();
    m_mazeGraphic->drawTextures();
}

QVector<QString> View::getOpenGLVersionInfo() {
    static QVector<QString> openGLVersionInfo;
    if (openGLVersionInfo.empty()) {
        QString glType = context()->isOpenGLES() ? "OpenGL ES" : "OpenGL";
        QString glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        QString glProfile;
        switch (format().profile()) {
            case QSurfaceFormat::NoProfile:
                glProfile = "NoProfile";
                break;
            case QSurfaceFormat::CoreProfile:
                glProfile = "CoreProfile";
                break;
            case QSurfaceFormat::CompatibilityProfile:
                glProfile = "CompatibilityProfile";
                break;
        }
        openGLVersionInfo = {glType, glVersion, glProfile};
    }
    return openGLVersionInfo;
}

// TODO: MACK
BufferInterface* View::getBufferInterface() {
    return m_bufferInterface;
}

void View::initOpenGLLogger() {
    if (m_openGLLogger.initialize()) {
        m_openGLLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
        m_openGLLogger.enableMessages();
        m_openGLLogger.disableMessages(
            QOpenGLDebugMessage::AnySource,
            QOpenGLDebugMessage::AnyType,
            QOpenGLDebugMessage::NotificationSeverity
        );
    }
}

void View::initializeGL() {

    // Contains all initialization that requires an OpenGL context

    // First, initialize the logger
    initOpenGLLogger();

    // Make it possible to call gl functions directly
    initializeOpenGLFunctions();

    // Set some gl values
    glClearColor(0.1, 0.0, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, S()->wireframeMode() ? GL_LINE : GL_FILL);

    // Initialize the polygon and texture programs
    initPolygonProgram();
    initTextureProgram();
}

void View::paintGL() {

    // In order to ensure we're sleeping the correct amount of time, we time
    // the drawing operation and take it into account when we sleep.
    double start(SimUtilities::getHighResTimestamp());

    // TODO: MACK - this shouldn't be here :/
    // First, clear fog as necessary
    // if (m_controllerManager->getDynamicOptions().automaticallyClearFog) {
    if (true) {
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

    // Draw the tiles
    drawFullAndZoomedMaps(
        currentMouseTranslation,
        currentMouseRotation,
        &m_polygonProgram,
        &m_polygonVAO,
        0,
        3 * mouseTrianglesStartingIndex
    );

    // Overlay the tile text
    drawFullAndZoomedMaps(
        currentMouseTranslation,
        currentMouseRotation,
        &m_textureProgram,
        &m_textureVAO,
        0,
        3 * m_textureCpuBuffer.size()
    );

    // Draw the mouse
    drawFullAndZoomedMaps(
        currentMouseTranslation,
        currentMouseRotation,
        &m_polygonProgram,
        &m_polygonVAO,
        3 * mouseTrianglesStartingIndex,
        3 * (m_graphicCpuBuffer.size() - mouseTrianglesStartingIndex)
    );

    // Disable scissoring so that the glClear can take effect, and so that
    // drawn text isn't clipped at all
    glDisable(GL_SCISSOR_TEST);

    // Draw the window header
    // TODO: MACK
    // m_header->draw();

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

void View::resizeGL(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    m_header->updateWindowSize(width, height);
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
}

void View::initTextureProgram() {

	m_textureProgram.addShaderFromSourceCode(
		QOpenGLShader::Vertex,
		R"(
            uniform mat4 transformationMatrix;
            attribute vec2 coordinate;
            attribute vec2 inTextureCoordinate;
            varying vec2 outTextureCoordinate;
            void main() {
                gl_Position = transformationMatrix * vec4(coordinate, 0.0, 1.0);
                outTextureCoordinate = inTextureCoordinate;
            }
		)"
	);
	m_textureProgram.addShaderFromSourceCode(
		QOpenGLShader::Fragment,
		R"(
            uniform sampler2D texture;
            varying vec2 outTextureCoordinate;
            void main() {
                gl_FragColor = texture2D(texture, outTextureCoordinate);
            }
		)"
	);
	m_textureProgram.link();
	m_textureProgram.bind();

	m_textureVAO.create();
	m_textureVAO.bind();

	m_textureVBO.create();
	m_textureVBO.bind();
	m_textureVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);

	m_textureProgram.enableAttributeArray("coordinate");
	m_textureProgram.setAttributeBuffer(
        "coordinate", // name
        GL_DOUBLE, // type
        0, // offset (bytes)
        2, // tupleSize (number of elements in the attribute array)
        4 * sizeof(double) // stride (bytes between vertices)
    );

	m_textureProgram.enableAttributeArray("inTextureCoordinate");
	m_textureProgram.setAttributeBuffer(
        "inTextureCoordinate", // name
        GL_DOUBLE, // type
        2 * sizeof(double), // offset (bytes)
        2, // tupleSize (number of elements in the attribute array)
        4 * sizeof(double) // stride (bytes between vertices)
    );

    // Load the bitmap texture into the texture atlas
    QString tileTextFontImagePath =
        Directory::get()->getResImgsDirectory() + P()->tileTextFontImage();
    if (!QFile::exists(tileTextFontImagePath)) {
        qCritical().noquote().nospace()
            << "Could not find font image file \"" << P()->tileTextFontImage()
            << "\" in \"" << Directory::get()->getResImgsDirectory() << "\".";
        SimUtilities::quit();
    }
    m_textureAtlas = new QOpenGLTexture(QImage(tileTextFontImagePath).mirrored());

	m_polygonVBO.release();
	m_polygonVAO.release();
	m_polygonProgram.release();
}

// TODO: MACK - move this somewhere else
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

    // Overwrite the polygon vertex buffer object data
    m_polygonVBO.bind();
	m_polygonVBO.allocate(
        &m_graphicCpuBuffer.front(),
        m_graphicCpuBuffer.size() * sizeof(TriangleGraphic)
    );
    m_polygonVBO.release();

    // Overwrite the texture vertex buffer object data
    m_textureVBO.bind();
	m_textureVBO.allocate(
        &m_textureCpuBuffer.front(),
        m_textureCpuBuffer.size() * sizeof(TriangleTexture)
    );
    m_textureVBO.release();
}

void View::drawFullAndZoomedMaps(
        const Coordinate& currentMouseTranslation,
        const Angle& currentMouseRotation,
        QOpenGLShaderProgram* program,
        QOpenGLVertexArrayObject* vao,
        int vboStartingIndex,
        int count) {

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
    if (program == &m_textureProgram) {
        glActiveTexture(GL_TEXTURE0);
        m_textureAtlas->bind();
        program->setUniformValue("texture", 0);
    }

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

    // TODO: MACK - necessary?
    glScissor(fullMapPosition.first, fullMapPosition.second, fullMapSize.first, fullMapSize.second);
    program->setUniformValue("transformationMatrix", transformationMatrix);
    glDrawArrays(GL_TRIANGLES, vboStartingIndex, count);

    // Render the zoomed map
    // TODO: MACK
    auto matrix2 = TransformationMatrix::getZoomedMapTransformationMatrix(
        physicalMazeSize,
        zoomedMapPosition,
        zoomedMapSize,
        {m_windowWidth, m_windowHeight},
        Screen::get()->pixelsPerMeter(),
        S()->zoomedMapScale(),
        S()->rotateZoomedMap(),
        m_model->getMouse()->getInitialTranslation(),
        currentMouseTranslation,
        currentMouseRotation
    );
    QMatrix4x4 transformationMatrix2(
        matrix2.at(0), matrix2.at(1), matrix2.at(2), matrix2.at(3),
        matrix2.at(4), matrix2.at(5), matrix2.at(6), matrix2.at(7),
        matrix2.at(8), matrix2.at(9), matrix2.at(10), matrix2.at(11),
        matrix2.at(12), matrix2.at(13), matrix2.at(14), matrix2.at(15)
    );

    // TODO: MACK - necessary?
    glScissor(zoomedMapPosition.first, zoomedMapPosition.second, zoomedMapSize.first, zoomedMapSize.second);
    program->setUniformValue("transformationMatrix", transformationMatrix2);
    glDrawArrays(GL_TRIANGLES, vboStartingIndex, count);

    // If it's the texture program, we should additionally unbind the texture
    if (program == &m_textureProgram) {
        m_textureAtlas->release();
    }

    // Stop using the program and vertex array object
    vao->release();
}

} // namespace mms
