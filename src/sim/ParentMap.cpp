#include "Map.h"

#include <QFile>
#include <QPair>

#include "Directory.h"
#include "FontImage.h"
#include "Layout.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "Screen.h"
#include "State.h"
#include "TransformationMatrix.h"
#include "units/Seconds.h"

namespace mms {

Map::Map(const Model* model, Lens* lens, QWidget* parent) :
        QOpenGLWidget(parent),
        m_model(model),
        m_lens(lens) {

    // Ensure that we continuously refresh the widget
	connect(
        this, &Map::frameSwapped,
        this, static_cast<void (Map::*)()>(&Map::update)
    );
}

QVector<QString> Map::getOpenGLVersionInfo() {
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

void Map::initOpenGLLogger() {
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

void Map::initializeGL() {

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

void Map::paintGL() {
    // TODO: MACK - repaint the map
}

void Map::resizeGL(int width, int height) {
    // TODO: MACK - do nothing
}

void Map::initPolygonProgram() {

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

void Map::initTextureProgram() {

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
    m_textureAtlas = new QOpenGLTexture(
        QImage(FontImage::get()->imageFilePath()).mirrored());

	m_polygonVBO.release();
	m_polygonVAO.release();
	m_polygonProgram.release();
}

void Map::repopulateVertexBufferObjects() {

    // TODO: MACK - we shouldn't need literal TriangleGraphic and
    // TriangleTexture here - these should be method calls on the lens object

    // Overwrite the polygon vertex buffer object data
    m_polygonVBO.bind();
	m_polygonVBO.allocate(
        &(m_lens->getGraphicCpuBuffer()->front()),
        m_lens->getGraphicCpuBuffer()->size() * sizeof(TriangleGraphic)
    );
    m_polygonVBO.release();

    // Overwrite the texture vertex buffer object data
    m_textureVBO.bind();
	m_textureVBO.allocate(
        &(m_lens->getTextureCpuBuffer()->front()),
        m_lens->getTextureCpuBuffer()->size() * sizeof(TriangleTexture)
    );
    m_textureVBO.release();
}

} // namespace mms
