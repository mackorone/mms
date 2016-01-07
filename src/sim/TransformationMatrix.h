#pragma once

#include <vector>

#include "units/Cartesian.h"
#include "units/Degrees.h"

namespace sim {

class TransformationMatrix {

public:

    // This class is not constructible
    TransformationMatrix() = delete;

    // Retrieve the 4x4 transformation matrix for the full map
    static std::vector<float> getFullMapTransformationMatrix(
        const Distance& wallWidth,
        std::pair<double, double> physicalMazeSize,
        std::pair<int, int> fullMapPosition,
        std::pair<int, int> fullMapSize,
        std::pair<int, int> windowSize);

    // Retrieve the 4x4 transformation matrices for zoomed map
    static std::vector<float> getZoomedMapTransformationMatrix(
        std::pair<double, double> physicalMazeSize,
        std::pair<int, int> zoomedMapPosition,
        std::pair<int, int> zoomedMapSize,
        std::pair<int, int> windowSize,
        double screenPixelsPerMeter,
        double zoomedMapScale,
        bool rotateZoomedMap,
        const Coordinate& initialMouseTranslation,
        const Coordinate& currentMouseTranslation,
        const Angle& currentMouseRotation);

private:

    // Maps a pixel coordinate ((0,0) in the bottom left, (width, height) in the upper right)
    // to the OpenGL coordinate system ((-1,-1) in the bottom left, (1,1) in the upper right)
    static std::pair<double, double> mapPixelCoordinateToOpenGlCoordinate(
        std::pair<double, double> coordinate,
        std::pair<int, int> windowSize);

    // Multiplies two 4x4 matrices together
    static std::vector<float> multiply4x4Matrices(
        std::vector<float> left,
        std::vector<float> right);

};

} // namespace sim
