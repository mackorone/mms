#pragma once

#include <glut/glut.h>
#include <tdogl/Program.h>
#include <tdogl/Texture.h>

#include "GlutFunctions.h"
#include "Header.h"
#include "MazeGraphic.h"
#include "Model.h"
#include "MouseGraphic.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"
#include "World.h"

namespace sim {

class View {

public:
    View(Model* model, int argc, char* argv[], const GlutFunctions& functions);
    MazeGraphic* getMazeGraphic();
    MouseGraphic* getMouseGraphic();

    void refresh();
    void updateWindowSize(int width, int height);
    std::set<char> getAllowableTileTextCharacters();
    void initTileGraphicText(std::pair<int, int> tileGraphicTextMaxSize);
    void passWorldPointerToHeader(const World* world);

private:

    // The model and graphic objects
    Model* m_model;
    MazeGraphic* m_mazeGraphic;
    MouseGraphic* m_mouseGraphic;

    // CPU-side buffers, and interface
    std::vector<TriangleGraphic> m_graphicCpuBuffer;
    std::vector<TriangleTexture> m_textureCpuBuffer;
    BufferInterface* m_bufferInterface;

    // The window size, in pixels
    int m_windowWidth;
    int m_windowHeight;

    // The number of pixels per meter of screen
    double m_screenPixelsPerMeter;

    // Allowable tile text characters
    std::set<char> m_allowableTileTextCharacters;

    // Window header object
    Header* m_header;

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
