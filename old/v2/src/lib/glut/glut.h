#include <GL/glew.h>
#include <GL/freeglut.h>

// XXX: This logic was previously necessary to use the simulator on Mac OSX.
// Apparently it's not needed anymore. I'll delete it once I'm sure.
#if(0)
    #ifdef __APPLE__
        #include <GLUT/glut.h>
    #else
        #include <GL/freeglut.h>
    #endif
#endif
