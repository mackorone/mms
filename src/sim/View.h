#pragma once

#include <QMap>
#include <QPair>
#include <QSet>
#include <QVector>

#include <glut/glut.h>
#include <tdogl/Program.h>
#include <tdogl/Texture.h>

#include "GlutFunctions.h"
#include "Header.h"
#include "MazeGraphic.h"
#include "Model.h"
#include "MouseGraphic.h"
#include "StaticMouseAlgorithmOptions.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"

namespace mms {

// We have to forward declare the class (as opposed to including it) so as to
// avoid a circular dependency; Controller.h already includes this file
class Controller;

class View {

public:
    View(Model* model, int argc, char* argv[], const GlutFunctions& functions);

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

    // Used to determine whether or not to automatically clear fog
    Controller* m_controller;

    // Polygon program variables
    tdogl::Program* m_polygonProgram;
    GLuint m_polygonVertexArrayObjectId;
    GLuint m_polygonVertexBufferObjectId;

    // Texture program variables
    tdogl::Texture* m_textureAtlas;
    tdogl::Program* m_textureProgram;
    GLuint m_textureVertexArrayObjectId;
    GLuint m_textureVertexBufferObjectId;

    // Initialize all of the graphics
    void initGraphics(int argc, char* argv[], const GlutFunctions& functions);
    void initPolygonProgram();
    void initTextureProgram();

    // Drawing helper methods
    void repopulateVertexBufferObjects();
    void drawFullAndZoomedMaps(
        const Coordinate& currentMouseTranslation, const Angle& currentMouseRotation,
        tdogl::Program* program, int vaoId, int vboStartingIndex, int vboEndingIndex);

};

} // namespace mms
