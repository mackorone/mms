#pragma once

#include <QMap>
#include <QOpenGLBuffer> 
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram> 
#include <QOpenGLTexture> 
#include <QOpenGLVertexArrayObject> 
#include <QOpenGLWidget>
#include <QPair>
#include <QSet>
#include <QVector>

#include "Header.h"
#include "MazeGraphic.h"
#include "Model.h"
#include "MouseGraphic.h"
#include "StaticMouseAlgorithmOptions.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"

namespace mms {

// TODO: MACK - break out the buffers into a separate class, call it View

// Inheriting from QOpenGLFunctions allows us to call the gl functions directly
class View : public QOpenGLWidget, protected QOpenGLFunctions {

	Q_OBJECT

public:

    View(Model* model, QWidget* parent = 0);

    MazeGraphic* getMazeGraphic();
    MouseGraphic* getMouseGraphic();

    QSet<QChar> getAllowableTileTextCharacters();
    void initTileGraphicText(int numRows, int numCols);

    QVector<QString> getOpenGLVersionInfo();

    // TODO: MACK
    BufferInterface* getBufferInterface();

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:

    // Vector of OpenGL version info
    QVector<QString> m_openglVersionInfo;

    // Logger of OpenGL warnings and errors
	QOpenGLDebugLogger m_openGLLogger;
	void initOpenGLLogger();

    // CPU-side buffers and the interface to modify them
    QVector<TriangleGraphic> m_graphicCpuBuffer;
    QVector<TriangleTexture> m_textureCpuBuffer;
    BufferInterface* m_bufferInterface;

    // The model and graphic objects
    Model* m_model;
    MazeGraphic* m_mazeGraphic;
    MouseGraphic* m_mouseGraphic;

    // The window size, in pixels
    int m_windowWidth;
    int m_windowHeight;

    // A map from char to x and y location in the font image
    QMap<QChar, QPair<double, double>> m_fontImageMap;
    QMap<QChar, QPair<double, double>> getFontImageMap();

    // Window header object
    Header* m_header;

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
    void drawFullAndZoomedMaps(
        const Coordinate& currentMouseTranslation,
		const Angle& currentMouseRotation,
        QOpenGLShaderProgram* program,
		QOpenGLVertexArrayObject* vao,
		int vboStartingIndex,
		int count);
};

} // namespace mms
