#pragma once

#include <QTimer>

#include <QOpenGLBuffer>
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

namespace mms {

class Example : public QOpenGLWidget, protected QOpenGLFunctions {

    Q_OBJECT

public:

    Example(QWidget *parent);

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:

    QOpenGLDebugLogger m_logger;
    void initLogger();

    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;
    void initShader();

    QVector<float> m_vertices;
    QOpenGLBuffer m_vbo;
    void repopulateBuffer();

    void printVersionInformation();

    
private slots:

    void onMessageLogged(QOpenGLDebugMessage);

};

} // namespace mms
