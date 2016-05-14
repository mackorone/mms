#pragma once

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

class IMouseAlgorithm;

namespace sim {

class View {

public:
    View(Model* model, int argc, char* argv[], const GlutFunctions& functions);

    MazeGraphic* getMazeGraphic();
    MouseGraphic* getMouseGraphic();

    void setMouseAlgorithmAndOptions(
        IMouseAlgorithm* mouseAlgorithm,
        StaticMouseAlgorithmOptions options);

    void refresh();
    void updateWindowSize(int width, int height);

    std::set<char> getAllowableTileTextCharacters();
    void initTileGraphicText();

    void keyPress(unsigned char key, int x, int y);
    void specialKeyPress(int key, int x, int y);
    void specialKeyRelease(int key, int x, int y);

private:

    // CPU-side buffers, and interface
    std::vector<TriangleGraphic> m_graphicCpuBuffer;
    std::vector<TriangleTexture> m_textureCpuBuffer;
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
    std::map<char, std::pair<double, double>> m_fontImageMap;
    std::map<char, std::pair<double, double>> getFontImageMap();

    // Window header object
    Header* m_header;

    // Used to determine whether or not to automatically clear fog
    IMouseAlgorithm* m_mouseAlgorithm;
    StaticMouseAlgorithmOptions m_options;

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

} // namespace sim
