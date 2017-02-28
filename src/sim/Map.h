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
#include "Maze.h"
#include "MazeView.h"
#include "Mouse.h"

namespace mms {

class Map : public QOpenGLWidget, protected QOpenGLFunctions {
    
    // NOTE: Inheriting from QOpenGLFunctions allows
    // us to call the OpenGL functions directly

	Q_OBJECT

public:

    Map(QWidget* parent = 0);

    void setMaze(const Maze* maze);
    void setMouse(const Mouse* mouse);
	void setView(const MazeView* view);

    // Retrieves OpenGL version info
    QVector<QString> getOpenGLVersionInfo();

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:

    // Logger of OpenGL warnings and errors
	QOpenGLDebugLogger m_openGLLogger;
	void initOpenGLLogger();

    // The maze, mouse, and current view
	const Maze* m_maze;
    const Mouse* m_mouse;
	const MazeView* m_view;

	// TODO: MACK - how do I draw the mouse

    // The map's window size, in pixels
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
