#include "Example.h"

#include "Logging.h"

namespace mms {

Example::Example(QWidget *parent) : QOpenGLWidget(parent) {
}

void Example::initializeGL() {

    initializeOpenGLFunctions();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_vertices = {
		 60.0,  10.0, 0.0,
	 	110.0, 110.0, 0.0,
		 10.0, 110.0, 0.0,
	};

    initLogger();
    initShader();
}

void Example::resizeGL(int w, int h) {
    // TODO: MACK
}

void Example::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT);

	repopulateBuffer();

	m_program.bind();
	//m_vao.bind();

	////
	QColor color(0, 255, 0, 255);
	QMatrix4x4 pmvMatrix;
	pmvMatrix.ortho(rect());

	m_program.setUniformValue("matrix", pmvMatrix);
	m_program.setUniformValue("color", color);

	m_program.enableAttributeArray("vertex");
	m_program.setAttributeArray("vertex", &m_vertices.front(), 3);
	////

	glDrawArrays(GL_TRIANGLES, 0, 3);

	////
	m_program.disableAttributeArray("vertex");
	////

	//m_vao.release();
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
			attribute highp vec4 vertex;
			uniform highp mat4 matrix;
			void main(void) {
			   gl_Position = matrix * vertex;
			}
		)"
	);
	m_program.addShaderFromSourceCode(
		QOpenGLShader::Fragment,
		R"(
			uniform mediump vec4 color;
			void main(void) {
			   gl_FragColor = color;
			}
		)"
	);
	m_program.link();

	// Create the GPU side buffer
	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(m_vertices.size() * sizeof(float));
	m_vbo.release();

	// m_vao.create();
	// QColor color(0, 255, 0, 255);
	// QMatrix4x4 pmvMatrix;
	// pmvMatrix.ortho(rect());
	// m_program.setUniformValue("matrix", pmvMatrix);
	// m_program.setUniformValue("color", color);
	// m_vao.bind();
	// m_vao.release();
}

void Example::repopulateBuffer() {
	// Copies data from the CPU to the GPU
	// m_program.enableAttributeArray("vertex");
	// m_program.setAttributeArray("vertex", &m_vertices.front(), 3);
	// m_program.disableAttributeArray("vertex");
}

void Example::onMessageLogged(QOpenGLDebugMessage message) {
    qDebug() << message;
}

} // namespace mms
