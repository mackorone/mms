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
// avoid a circular dependency; Controller.h already includes this file
class Controller;

// TODO: MACK - private inheritance here?
class View : public QOpenGLWidget, protected QOpenGLFunctions {

	Q_OBJECT

public:

    View(
		Model* model,
		int argc,
		char* argv[],
		QWidget* parent = 0);

    MazeGraphic* getMazeGraphic();
    MouseGraphic* getMouseGraphic();

    void setController(Controller* controller);

    void refresh();
    void updateWindowSize(int width, int height);

    QSet<QChar> getAllowableTileTextCharacters();
    void initTileGraphicText();

    void keyPress(unsigned char key, int x, int y);
    void specialKeyPress(int key, int x, int y);
    void specialKeyRelease(int key, int x, int y);

// TODO: MACK - do these need to be protected?
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:

    // CPU-side buffers, and interface
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

    // The number of pixels per meter of screen
    double m_screenPixelsPerMeter;

    // A map from char to x and y location in the font image
    QMap<QChar, QPair<double, double>> m_fontImageMap;
    QMap<QChar, QPair<double, double>> getFontImageMap();

    // Window header object
    Header* m_header;

	// TODO: MACK - this shouldn't be in here...
    // Used to determine whether or not to automatically clear fog
    Controller* m_controller;

    // Polygon program variables
	// TODO: MACK - do I need this anymore?
    // tdogl::Program* m_polygonProgram;
    GLuint m_polygonVertexArrayObjectId;
    GLuint m_polygonVertexBufferObjectId;

    // Texture program variables
    // tdogl::Texture* m_textureAtlas;
    // tdogl::Program* m_textureProgram;
    GLuint m_textureVertexArrayObjectId;
    GLuint m_textureVertexBufferObjectId;

	//////////

	// TODO: MACK - rename these
	QOpenGLShaderProgram m_polygonProgram;
	QOpenGLVertexArrayObject m_polygonVertexArrayObject;
	QOpenGLBuffer m_polygonVertexBufferObject;

	// TODO: MACK - rename these
	QOpenGLTexture* m_textureAtlas;
	QOpenGLShaderProgram m_textureProgram;
	QOpenGLVertexArrayObject m_textureVertexArrayObject;
	QOpenGLBuffer m_textureVertexBufferObject;

	//////////

    // Initialize all of the graphics
    void initGraphics(int argc, char* argv[]);
    void initPolygonProgram();
    void initTextureProgram();

    // Drawing helper methods
    void repopulateVertexBufferObjects();
	// TODO: MACK
    // void drawFullAndZoomedMaps(
    //     const Coordinate& currentMouseTranslation, const Angle& currentMouseRotation,
    //     tdogl::Program* program, int vaoId, int vboStartingIndex, int vboEndingIndex);
    void drawFullAndZoomedMaps(
        const Coordinate& currentMouseTranslation,
		const Angle& currentMouseRotation,
        QOpenGLShaderProgram* program,
		QOpenGLVertexArrayObject* vao,
		int vboStartingIndex,
		int vboEndingIndex);

	// TODO: MACK
	QOpenGLDebugLogger m_logger;

public slots:
	void onMessageLogged(QOpenGLDebugMessage);
};

} // namespace mms
