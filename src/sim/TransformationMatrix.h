#pragma once

#include <QPair>
#include <QVector>

#include "units/Coordinate.h"

namespace mms {

class TransformationMatrix {

public:

    // This class is not constructible
    TransformationMatrix() = delete;

    // Retrieve the 4x4 transformation matrix for the full map
    static QVector<float> getFullMapTransformationMatrix(
        const Distance& wallWidth,
        QPair<double, double> physicalMazeSize,
        QPair<int, int> fullMapPosition,
        QPair<int, int> fullMapSize,
        QPair<int, int> windowSize);

    // Retrieve the 4x4 transformation matrices for zoomed map
    static QVector<float> getZoomedMapTransformationMatrix(
        QPair<double, double> physicalMazeSize,
        QPair<int, int> zoomedMapPosition,
        QPair<int, int> zoomedMapSize,
        QPair<int, int> windowSize,
        double screenPixelsPerMeter,
        double zoomedMapScale,
        bool rotateZoomedMap,
        const Coordinate& initialMouseTranslation,
        const Coordinate& currentMouseTranslation,
        const Angle& currentMouseRotation);

private:

    // Maps a pixel coordinate ((0,0) in the bottom left, (width, height) in the upper right)
    // to the OpenGL coordinate system ((-1,-1) in the bottom left, (1,1) in the upper right)
    static QPair<double, double> mapPixelCoordinateToOpenGlCoordinate(
        QPair<double, double> coordinate,
        QPair<int, int> windowSize);

    // Multiplies two 4x4 matrices together
    static QVector<float> multiply4x4Matrices(
        QVector<float> left,
        QVector<float> right);

};

} 
