#pragma once

#include <glut/glut.h>

// Forward declarations
namespace tdogl {
    class Program;
    class Texture;
}
namespace sim {
    class IMouseAlgorithm;
    class MazeGraphic;
    class Mouse;
    class MouseGraphic;
    class MouseInterface;
    class TextDrawer;
    class World;
}

namespace sim {

class Driver {

public:

    // The Driver class is not constructible
    Driver() = delete;

    // The name "drive" is just supposed to be punny
    static void drive(int argc, char* argv[]);

private:

    // Simulation objects
    static Mouse*          m_mouse;
    static MazeGraphic*    m_mazeGraphic;
    static MouseGraphic*   m_mouseGraphic;
    static MouseInterface* m_mouseInterface;
    static World*          m_world;

    // Micromouse algorithm object
    static IMouseAlgorithm* m_algorithm;

    // Text drawing object
    static TextDrawer* m_textDrawer;

    // Polygon program variables
    static tdogl::Program* m_polygonProgram;
    static GLuint          m_polygonVertexArrayObjectId;
    static GLuint          m_polygonVertexBufferObjectId;

    // Texture program variables
    static tdogl::Texture* m_textureAtlas;
    static tdogl::Program* m_textureProgram;
    static GLuint          m_textureVertexArrayObjectId;
    static GLuint          m_textureVertexBufferObjectId;

    // TODO: put some good descriptions here...
    static void bootstrap();

    static void initSimObjects();

    static void LoadTriangle(); // TODO: kill this

    static void draw();
    static void keyPress(unsigned char key, int x, int y);
    static void specialKeyPress(int key, int x, int y);
    static void specialKeyRelease(int key, int x, int y);

    static void initPolygonProgram();
    static void initTextureProgram();
    static void initGraphics(int argc, char* argv[]);

    static void initMouseAlgo();
};

} // namespace sim
