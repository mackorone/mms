#pragma once

#include <glut/glut.h> // TODO: MACK - do I need this??
#include <tdogl/Program.h>
#include <tdogl/Texture.h>

#include "GlutFunctions.h"
#include "MazeGraphic.h"
#include "Model.h"
#include "MouseGraphic.h"
#include "TextDrawer.h"

namespace sim {

class View {

public:
    View(Model* model, int argc, char* argv[], GlutFunctions functions);

    MazeGraphic* getMazeGraphic(); // TODO: MACK - get rid of these eventually
    MouseGraphic* getMouseGraphic(); // TODO: MACK - get rid of these eventually

    // TODO: MACK
    void draw();

private:

    Model* m_model;

    MazeGraphic* m_mazeGraphic;
    MouseGraphic* m_mouseGraphic;

// TODO: MACK -----

    // TODO: MACK
    void initGraphics(int argc, char* argv[], GlutFunctions functions);

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

    // Callback functions
    // Initialize all of the graphics
    void initPolygonProgram();
    void initTextureProgram();

    // Drawing helper methods
    void repopulateVertexBufferObjects();
    void drawFullAndZoomedMaps(
        const Coordinate& currentMouseTranslation, const Angle& currentMouseRotation,
        tdogl::Program* program, int vaoId, int vboStartingIndex, int vboEndingIndex);


};

} // namespace sim
