#pragma once

#include <glut/glut.h>
#include <tdogl/Program.h>
#include <tdogl/Texture.h>

#include "GlutFunctions.h"
#include "MazeGraphic.h"
#include "Model.h"
#include "MouseGraphic.h"
#include "TextDrawer.h"
#include "TriangleGraphic.h"
#include "TriangleTexture.h"

namespace sim {

class View {

public:
    View(Model* model, int argc, char* argv[], const GlutFunctions& functions);
    MazeGraphic* getMazeGraphic();
    MouseGraphic* getMouseGraphic();
    void draw();

    // Get and set the window size, in pixels
    std::pair<int, int> getWindowSize();
    void setWindowSize(int width, int height);

    // Retrieve the 4x4 transformation matrices for each map
    // TODO: MACK: Clean these implementations up
    std::vector<float> getFullMapTransformationMatrix(
        std::pair<double, double> physicalMazeSize,
        std::pair<int, int> fullMapPosition,
        std::pair<int, int> fullMapSize);

    std::vector<float> getZoomedMapTransformationMatrix(
        std::pair<double, double> physicalMazeSize,
        std::pair<int, int> zoomedMapPosition,
        std::pair<int, int> zoomedMapSize,
        const Coordinate& initialMouseTranslation,
        const Coordinate& currentMouseTranslation,
        const Angle& currentMouseRotation);

// TODO: MACK --------------------------- VIEW DATA

    // Fills the GRAPHIC_CPU_BUFFER and TEXTURE_CPU_BUFFER
    void insertIntoGraphicCpuBuffer(const Polygon& polygon, Color color, double alpha);
    void insertIntoTextureCpuBuffer();

    // These methods are inexpensive, and may be called many times
    void updateTileGraphicBaseColor(int x, int y, Color color);
    void updateTileGraphicWallColor(int x, int y, Direction direction, Color color, double alpha);
    void updateTileGraphicFog(int x, int y, double alpha);
    void updateTileGraphicText(const Tile* tile, int numRows, int numCols, int row, int col, char c);

    // Appends a mouse polygon to the GRAPHIC_CPU_BUFFER
    void drawMousePolygon(const Polygon& polygon, Color color, double sensorAlpha); // TODO: MACK - move

    // CPU-side buffers
    std::vector<TriangleGraphic> GRAPHIC_CPU_BUFFER; // TODO: MACK - move
    std::vector<TriangleTexture> TEXTURE_CPU_BUFFER; // TODO: MACK - move

    // Converts a polygon to a vector of triangle graphics or triangle textures
    std::vector<TriangleGraphic> polygonToTriangleGraphics(const Polygon& polygon, Color color, double alpha); // TODO: MACK - move

    // Retrieve the indices into the GRAPHIC_CPU_BUFFER for each specific type of Tile triangle
    int trianglesPerTile();
    int getTileGraphicBaseStartingIndex(int x, int y);
    int getTileGraphicWallStartingIndex(int x, int y, Direction direction);
    int getTileGraphicCornerStartingIndex(int x, int y, int cornerNumber);
    int getTileGraphicFogStartingIndex(int x, int y);

    // Retrieve the indices into the TEXTURE_CPU_BUFFER
    int getTileGraphicTextStartingIndex(int x, int y, int row, int col);

    // Returns the maximum number of rows and columns of text in a tile graphic
    std::pair<int, int> getTileGraphicTextMaxSize(); // TODO: MACK - shouldn't need

// TODO: MACK --------------------------- VIEW DATA

private:

    // TODO: MACK Refactor and reorganize all of this
    int m_mazeWidth; // TODO: MACK
    int m_mazeHeight; // TODO: MACK

    Model* m_model;
    MazeGraphic* m_mazeGraphic;
    MouseGraphic* m_mouseGraphic;

    // TODO: MACK - in pixels
    int m_windowWidth;
    int m_windowHeight;

    // Text drawing object
    TextDrawer* m_textDrawer;

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

// TODO: MACK - view data methods

    // Return the positions of the full map and zoomed map, in pixels
    std::pair<int, int> getFullMapPosition(int windowWidth, int windowHeight);
    std::pair<int, int> getZoomedMapPosition(int windowWidth, int windowHeight);

    // Return the width and height of the full map and zoomed map, in pixels
    std::pair<int, int> getFullMapSize(int windowWidth, int windowHeight);
    std::pair<int, int> getZoomedMapSize(int windowWidth, int windowHeight);

    // Maps a pixel coordinate ((0,0) in the bottom left, (width, height) in the upper right)
    // to the OpenGL coordinate system ((-1,-1) in the bottom left, (1,1) in the upper right)
    std::pair<double, double> mapPixelCoordinateToOpenGlCoordinate(
        double x, double y, int windowWidth, int windowHeight);

    // Multiplies two 4x4 matrices together
    std::vector<float> multiply4x4Matrices(std::vector<float> left, std::vector<float> right);

    // Returns the number of pixels per meter of the screen
    double getScreenPixelsPerMeter();

};

} // namespace sim
