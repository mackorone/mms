#pragma once

#include <QMap>
#include <QOpenGLBuffer> 
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram> 
#include <QOpenGLTexture> 
#include <QOpenGLVertexArrayObject> 
#include <QOpenGLWidget>
#include <QPair>
#include <QSet>
#include <QVector>

// TODO: MACK
#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>

#include "Header.h"
#include "MazeGraphic.h"
#include "Model.h"
#include "MouseGraphic.h"
#include "StaticMouseAlgorithmOptions.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"

namespace mms {

// TODO: MACK - we can get rid of this
// We have to forward declare the class (as opposed to including it) so as to
// avoid a circular dependency; ControllerManager.h already includes this file
class ControllerManager;

// Inheriting from QOpenGLFunctions allows us to call the gl functions directly
class View : public QOpenGLWidget, protected QOpenGLFunctions {

	Q_OBJECT

public:

    View(Model* model, QWidget* parent = 0);

    MazeGraphic* getMazeGraphic();
    MouseGraphic* getMouseGraphic();

    void setControllerManager(ControllerManager* controllerManager);

    QSet<QChar> getAllowableTileTextCharacters();
    void initTileGraphicText();

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:

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

	// TODO: MACK - this shouldn't be in here...
    // Used to determine whether or not to automatically clear fog
    ControllerManager* m_controllerManager;

    // Texture program variables
    // tdogl::Texture* m_textureAtlas;
    // tdogl::Program* m_textureProgram;
    GLuint m_textureVertexArrayObjectId;
    GLuint m_textureVertexBufferObjectId;

	//////////

	// TODO: MACK - rename these
	QOpenGLShaderProgram m_polygonProgram;
	QOpenGLVertexArrayObject m_polygonVAO;
	QOpenGLBuffer m_polygonVBO;

	// TODO: MACK - rename these
	QOpenGLTexture* m_textureAtlas;
	QOpenGLShaderProgram m_textureProgram;
	QOpenGLVertexArrayObject m_textureVAO;
	QOpenGLBuffer m_textureVBO;

	//////////

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

	// TODO: MACK - clean this up
	QOpenGLDebugLogger m_logger;
	void initLogger();
	void initShader();
    void printVersionInformation();
private slots:
	void onMessageLogged(QOpenGLDebugMessage);

};

} // namespace mms
