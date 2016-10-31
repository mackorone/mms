#include "Example.h"

#include "Logging.h"

namespace mms {

Example::Example(QWidget *parent) : QOpenGLWidget(parent) {
}

void Example::initializeGL() {

    initializeOpenGLFunctions();
	printVersionInformation();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// x, y, z
	m_vertices = {
		  0.0,   0.0, 0.0,
	 	100.0, 100.0, 0.0,
		  0.0, 100.0, 0.0,
	};

    initLogger();
    initShader();

	connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void Example::resizeGL(int w, int h) {
    // TODO: MACK
}

void Example::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);

	repopulateBuffer();

	m_program.bind();
	m_vao.bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);

	m_vao.release();
	m_program.release();
}

void Example::initLogger() {
    connect(
        &m_logger,
        SIGNAL(messageLogged(QOpenGLDebugMessage)),
        this,
        SLOT(onMessageLogged(QOpenGLDebugMessage))
    );
    if (m_logger.initialize()) {
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

void Example::initShader() {

	// Create the program
	m_program.addShaderFromSourceCode(
		QOpenGLShader::Vertex,
		R"(
			attribute vec4 vertex;
			uniform mat4 matrix;
			void main(void) {
			   gl_Position = matrix * vertex;
			}
		)"
	);
	m_program.addShaderFromSourceCode(
		QOpenGLShader::Fragment,
		R"(
			uniform vec4 color;
			void main(void) {
			   gl_FragColor = color;
			}
		)"
	);
	m_program.link();
	m_program.bind();

	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
	m_vbo.allocate(&m_vertices.front(), m_vertices.size() * sizeof(float));

	m_program.enableAttributeArray("vertex");
	m_program.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);

	QColor color(0, 255, 0, 255);
	QMatrix4x4 pmvMatrix;
	pmvMatrix.ortho(rect());
	m_program.setUniformValue("color", color);
	m_program.setUniformValue("matrix", pmvMatrix);

	m_vbo.release();
	m_vao.release();
	m_program.release();
}

void Example::repopulateBuffer() {

	static int i = 0;
	i = (i + 1) % 100;
	m_vertices[0] = static_cast<float>(i);

	m_vbo.bind();
	m_vbo.write(0, &m_vertices.front(), m_vertices.size() * sizeof(float));
	m_vbo.release();
}

void Example::onMessageLogged(QOpenGLDebugMessage message) {
    qDebug() << message;
}

void Example::printVersionInformation() {

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
