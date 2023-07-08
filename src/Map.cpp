#include "Map.h"

#include <QElapsedTimer>
#include <QFile>

#include "AssertMacros.h"
#include "Dimensions.h"
#include "FontImage.h"
#include "Logging.h"
#include "TransformationMatrix.h"

namespace mms {

Map::Map(QWidget *parent)
    : QOpenGLWidget(parent),
      m_maze(nullptr),
      m_view(nullptr),
      m_mouseGraphic(nullptr),
      m_windowWidth(0),
      m_windowHeight(0),
      m_textureAtlas(nullptr) {
  ASSERT_RUNS_JUST_ONCE();
}

void Map::setMaze(const Maze *maze) {
  ASSERT_TR(m_mouseGraphic == nullptr);
  m_maze = maze;
  m_view = nullptr;
}

void Map::setView(const MazeView *view) {
  if (view != nullptr) {
    ASSERT_FA(m_maze == nullptr);
  }
  m_view = view;
}

void Map::setMouseGraphic(const MouseGraphic *mouseGraphic) {
  if (mouseGraphic != nullptr) {
    ASSERT_FA(m_maze == nullptr);
    ASSERT_FA(m_view == nullptr);
  }
  m_mouseGraphic = mouseGraphic;
}

QStringList Map::getOpenGLVersionInfo() {
  static QStringList info;
  if (info.empty()) {
    QString glType = context()->isOpenGLES() ? "OpenGL ES" : "OpenGL";
    QString glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
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
    info.append(glType);
    info.append(glVersion);
    info.append(glProfile);
  }
  return info;
}

void Map::shutdown() {
  makeCurrent();
  m_openGLLogger.stopLogging();
}

void Map::initOpenGLLogger() {
  if (m_openGLLogger.initialize()) {
    m_openGLLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
    m_openGLLogger.enableMessages();
    m_openGLLogger.disableMessages(QOpenGLDebugMessage::AnySource,
                                   QOpenGLDebugMessage::AnyType,
                                   QOpenGLDebugMessage::NotificationSeverity);
  }
}

void Map::initializeGL() {
  // Contains all initialization that requires an OpenGL context

  // First, initialize the logger
  initOpenGLLogger();

  // Make it possible to call gl functions directly
  initializeOpenGLFunctions();

  // Set some gl values
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  // Initialize the polygon and texture programs
  initPolygonProgram();
  initTextureProgram();
}

void Map::paintGL() {
  // TODO: upforgrabs
  // Optimize this code
  //
  // QElapsedTimer timer;
  // timer.start();

  // If the view hasn't been set yet, just draw black
  if (m_view == nullptr) {
    glClear(GL_COLOR_BUFFER_BIT);
    return;
  }

  QVector<TriangleGraphic> mouseBuffer;
  if (m_mouseGraphic != nullptr) {
    mouseBuffer = m_mouseGraphic->draw();
  }

  // Re-populate both vertex buffer objects
  repopulateVertexBufferObjects(mouseBuffer);

  // Draw the tiles
  drawMap(&m_polygonProgram, &m_polygonVAO, 0,
          3 * m_view->getGraphicCpuBuffer()->size());

  // Overlay the tile text
  if (m_textureAtlas != nullptr) {
    drawMap(&m_textureProgram, &m_textureVAO, 0,
            3 * m_view->getTextureCpuBuffer()->size());
  }

  // Draw the mouse
  drawMap(&m_polygonProgram, &m_polygonVAO,
          3 * m_view->getGraphicCpuBuffer()->size(), 3 * mouseBuffer.size());

  // TODO: upforgrabs
  // Optimize this code
  //
  // qDebug() << timer.nsecsElapsed();
}

void Map::resizeGL(int width, int height) {
  m_windowWidth = width;
  m_windowHeight = height;
}

void Map::initPolygonProgram() {
  m_polygonProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           R"(
            uniform mat4 transformationMatrix;
            attribute vec2 coordinate;
            attribute vec4 inColor;
            varying vec4 outColor;
            void main(void) {
                gl_Position = transformationMatrix * vec4(coordinate, 0.0, 1.0);
                outColor = inColor;
            }
        )");
  m_polygonProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           R"(
            varying vec4 outColor;
            void main(void) {
               gl_FragColor = outColor;
            }
        )");
  m_polygonProgram.link();
  m_polygonProgram.bind();

  m_polygonVAO.create();
  m_polygonVAO.bind();

  m_polygonVBO.create();
  m_polygonVBO.bind();
  m_polygonVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  m_polygonProgram.enableAttributeArray("coordinate");
  m_polygonProgram.setAttributeBuffer(
      "coordinate",  // name
      GL_FLOAT,      // type
      0,             // offset (bytes)
      2,             // tupleSize (number of elements in the attribute array)
      2 * sizeof(float) +
          4 * sizeof(unsigned char)  // stride (bytes between vertices)
  );

  m_polygonProgram.enableAttributeArray("inColor");
  m_polygonProgram.setAttributeBuffer(
      "inColor",          // name
      GL_UNSIGNED_BYTE,   // type
      2 * sizeof(float),  // offset (bytes)
      4,  // tupleSize (number of elements in the attribute array)
      2 * sizeof(float) +
          4 * sizeof(unsigned char)  // stride (bytes between vertices)
  );

  m_polygonVBO.release();
  m_polygonVAO.release();
  m_polygonProgram.release();
}

void Map::initTextureProgram() {
  m_textureProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           R"(
            uniform mat4 transformationMatrix;
            attribute vec2 coordinate;
            attribute vec2 inTextureCoordinate;
            varying vec2 outTextureCoordinate;
            void main() {
                gl_Position = transformationMatrix * vec4(coordinate, 0.0, 1.0);
                outTextureCoordinate = inTextureCoordinate;
            }
        )");
  m_textureProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           R"(
            uniform sampler2D texture;
            varying vec2 outTextureCoordinate;
            void main() {
                gl_FragColor = texture2D(texture, outTextureCoordinate);
            }
        )");
  m_textureProgram.link();
  m_textureProgram.bind();

  m_textureVAO.create();
  m_textureVAO.bind();

  m_textureVBO.create();
  m_textureVBO.bind();
  m_textureVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);

  m_textureProgram.enableAttributeArray("coordinate");
  m_textureProgram.setAttributeBuffer(
      "coordinate",  // name
      GL_FLOAT,      // type
      0,             // offset (bytes)
      2,             // tupleSize (number of elements in the attribute array)
      4 * sizeof(float)  // stride (bytes between vertices)
  );

  m_textureProgram.enableAttributeArray("inTextureCoordinate");
  m_textureProgram.setAttributeBuffer(
      "inTextureCoordinate",  // name
      GL_FLOAT,               // type
      2 * sizeof(float),      // offset (bytes)
      2,  // tupleSize (number of elements in the attribute array)
      4 * sizeof(float)  // stride (bytes between vertices)
  );

  // Load the bitmap texture into the texture atlas
  if (QFile::exists(FontImage::path())) {
    m_textureAtlas = new QOpenGLTexture(QImage(FontImage::path()).mirrored());
  } else {
    qWarning() << "Font image file does not exist:" << FontImage::path();
  }

  m_polygonVBO.release();
  m_polygonVAO.release();
  m_polygonProgram.release();
}

void Map::repopulateVertexBufferObjects(
    const QVector<TriangleGraphic> &mouseBuffer) {
  // Overwrite the polygon vertex buffer object data
  m_polygonVBO.bind();
  m_polygonVBO.allocate(
      sizeof(TriangleGraphic) *
      (m_view->getGraphicCpuBuffer()->size() + mouseBuffer.size()));
  // Write the maze
  m_polygonVBO.write(
      0, &(m_view->getGraphicCpuBuffer()->front()),
      sizeof(TriangleGraphic) * m_view->getGraphicCpuBuffer()->size());
  // Write the mouse
  if (!mouseBuffer.isEmpty()) {
    m_polygonVBO.write(
        sizeof(TriangleGraphic) * m_view->getGraphicCpuBuffer()->size(),
        &(mouseBuffer.front()), sizeof(TriangleGraphic) * mouseBuffer.size());
  }
  m_polygonVBO.release();

  // Overwrite the texture vertex buffer object data
  m_textureVBO.bind();
  m_textureVBO.allocate(
      &(m_view->getTextureCpuBuffer()->front()),
      sizeof(TriangleTexture) * m_view->getTextureCpuBuffer()->size());
  m_textureVBO.release();
}

void Map::drawMap(QOpenGLShaderProgram *program, QOpenGLVertexArrayObject *vao,
                  int vboStartingIndex, int count) {
  // Start using the program and vertex array object
  program->bind();
  vao->bind();

  // If it's the texture program, bind the texture and set the uniform
  if (program == &m_textureProgram) {
    glActiveTexture(GL_TEXTURE0);
    m_textureAtlas->bind();
    program->setUniformValue("texture", 0);
  }

  // TODO: upforgrabs
  // This should be QTransform
  QMatrix4x4 transformationMatrix = TransformationMatrix::get(
      m_maze->getWidth(), m_maze->getHeight(), m_windowWidth, m_windowHeight);

  program->setUniformValue("transformationMatrix", transformationMatrix);
  glDrawArrays(GL_TRIANGLES, vboStartingIndex, count);

  // If it's the texture program, we should additionally unbind the texture
  if (program == &m_textureProgram) {
    m_textureAtlas->release();
  }

  // Stop using the program and vertex array object
  vao->release();
}

}  // namespace mms
