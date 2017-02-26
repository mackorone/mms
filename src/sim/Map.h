#pragma once

#include <QOpenGLBuffer> 
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram> 
#include <QOpenGLTexture> 
#include <QOpenGLVertexArrayObject> 
#include <QOpenGLWidget>
#include <QVector>

#include "LayoutType.h"
#include "Lens.h"
#include "Maze.h"
#include "Mouse.h"

namespace mms {

class Map : public QOpenGLWidget, protected QOpenGLFunctions {
    
    // NOTE: Inheriting from QOpenGLFunctions allows
    // us to call the OpenGL functions directly

	Q_OBJECT

public:

    Map(QWidget* parent = 0);

    void setMaze(const Maze* maze); // TODO: MACK - should this remove the mouse - yes
    void setMouseAndLens(const Mouse* mouse, Lens* lens);

    // TODO: MACK - do we need this???
    QVector<QString> getOpenGLVersionInfo();

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:

    // Logger of OpenGL warnings and errors
	QOpenGLDebugLogger m_openGLLogger;
	void initOpenGLLogger();

    // The maze and mouse
    const Maze* m_maze;
    const Mouse* m_mouse;

    // TODO: MACK - figure out how to make const
    // The maze, as perceived by the mouse algo
    Lens* m_lens;
    // TODO: MACK
    // The maze, as it appears in real life
    Lens* m_truth;
    Lens* m_currentLens;

    // The window size, in pixels
    int m_windowWidth;
    int m_windowHeight;

    // Polygon program variables
	QOpenGLShaderProgram m_polygonProgram;
	QOpenGLVertexArrayObject m_polygonVAO;
	QOpenGLBuffer m_polygonVBO;

    // Texture program variables
	QOpenGLTexture* m_textureAtlas;
	QOpenGLShaderProgram m_textureProgram;
	QOpenGLVertexArrayObject m_textureVAO;
	QOpenGLBuffer m_textureVBO;

    // Initialize the graphics
    void initPolygonProgram();
    void initTextureProgram();

    // Drawing helper methods
    void repopulateVertexBufferObjects();
    void drawMap(
        LayoutType type,
        const Coordinate& currentMouseTranslation,
		const Angle& currentMouseRotation,
        QOpenGLShaderProgram* program,
		QOpenGLVertexArrayObject* vao,
		int vboStartingIndex,
		int count);
};

} // namespace mms
