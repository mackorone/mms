#pragma once

#include <glut/glut.h>
#include <tdogl/Program.h>
#include <tdogl/Texture.h>

#include "GlutFunctions.h"
#include "MazeGraphic.h"
#include "Model.h"
#include "MouseGraphic.h"
#include "TextDrawer.h"
#include "ViewData.h"

namespace sim {

class View {

public:
    View(Model* model, int argc, char* argv[], const GlutFunctions& functions);
    MazeGraphic* getMazeGraphic();
    MouseGraphic* getMouseGraphic();
    void draw();

    std::pair<int, int> getWindowSize(); // TODO: MACK
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

private:

    // TODO: MACK Refactor and reorganize all of this


    Model* m_model;
    ViewData* m_data; // TODO: MACK - call this something else...
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
