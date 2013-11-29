#ifndef GRAPHICSTOOLS_H_
#define GRAPHICSTOOLS_H_

// Converts points from pixel coordinates to opengl specification
inline float convertPoint(float coordinate){
    return ((coordinate / WINDOW_WIDTH) - 0.5f) * 2;
}

#endif // GRAPHICSTOOLS_H_
