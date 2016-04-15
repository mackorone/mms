#include "TransformationMatrix.h"

#include <algorithm>

#include "Assert.h"
#include "CPMath.h"

namespace sim {

std::vector<float> TransformationMatrix::getFullMapTransformationMatrix(
        const Distance& wallWidth,
        std::pair<double, double> physicalMazeSize,
        std::pair<int, int> fullMapPosition,
        std::pair<int, int> fullMapSize,
        std::pair<int, int> windowSize) {

    // The purpose of this function is to produce a 4x4 matrix which, when
    // applied to the physical coordinate within the vertex shader, transforms
    // it into an OpenGL coordinate for the full map. In this case, the zoomed
    // map always contains the entirety of the maze.

    // Step 1: The physical point (0,0) corresponds to the middle of the
    // bottom-left corner piece:
    //                                 |       |
    //                                 +-------+---
    //                                 |       |
    //                                 |   X   |
    //                                 |       |
    //                                 +-------+---
    //
    // However, we want to make sure that the entire maze is visible within the
    // map window. To ensure this, we first have to translate the physical
    // positions so that (0,0) actually refers to the bottom-left corner of the
    // bottom-left corner:
    //
    //                                 |       |
    //                                 +-------+---
    //                                 |       |
    //                                 |       |
    //                                 |       |
    //                                 X-------+---
    //
    std::vector<float> initialTranslationMatrix = {
        1.0, 0.0, 0.0, static_cast<float>(0.5 * wallWidth.getMeters()),
        0.0, 1.0, 0.0, static_cast<float>(0.5 * wallWidth.getMeters()),
        0.0, 0.0, 1.0,                                             0.0,
        0.0, 0.0, 0.0,                                             1.0,
    };

    // Ensure that the maze width and height always appear equally scaled.
    double physicalWidth = physicalMazeSize.first;
    double physicalHeight = physicalMazeSize.second;

    // Note that this is not literally the number of pixels per meter of the
    // screen. Rather, it's our desired number of pixels per simulation meter.
    double pixelsPerMeter = std::min(fullMapSize.first / physicalWidth, fullMapSize.second / physicalHeight);
    double pixelWidth = pixelsPerMeter * physicalWidth;
    double pixelHeight = pixelsPerMeter * physicalHeight;

    std::pair<double, double> openGlOrigin =
        mapPixelCoordinateToOpenGlCoordinate(std::make_pair(0, 0), windowSize);
    std::pair<double, double> openGlMazeSize =
        mapPixelCoordinateToOpenGlCoordinate(std::make_pair(pixelWidth, pixelHeight), windowSize);

    double openGlWidth = openGlMazeSize.first - openGlOrigin.first;
    double openGlHeight = openGlMazeSize.second - openGlOrigin.second;

    double horizontalScaling = openGlWidth / physicalWidth;
    double verticalScaling = openGlHeight / physicalHeight;

    std::vector<float> scalingMatrix = {
        static_cast<float>(horizontalScaling),                                 0.0, 0.0, 0.0,
                                          0.0, static_cast<float>(verticalScaling), 0.0, 0.0,
                                          0.0,                                 0.0, 1.0, 0.0,
                                          0.0,                                 0.0, 0.0, 1.0,
    };
    
    // Step 3: Construct the translation matrix. Note that here we ensure that
    // the maze is centered within the map boundaries.
    double pixelLowerLeftCornerX = fullMapPosition.first + 0.5 * (fullMapSize.first - pixelWidth);
    double pixelLowerLeftCornerY = fullMapPosition.second + 0.5 * (fullMapSize.second - pixelHeight);
    std::pair<double, double> openGlLowerLeftCorner =
        mapPixelCoordinateToOpenGlCoordinate(std::make_pair(pixelLowerLeftCornerX, pixelLowerLeftCornerY), windowSize);
    std::vector<float> translationMatrix = {
        1.0, 0.0, 0.0,  static_cast<float>(openGlLowerLeftCorner.first),
        0.0, 1.0, 0.0, static_cast<float>(openGlLowerLeftCorner.second),
        0.0, 0.0, 1.0,                                              0.0,
        0.0, 0.0, 0.0,                                              1.0,
    };

    // Step 4: Compose the matrices
    std::vector<float> transformationMatrix =
        multiply4x4Matrices(translationMatrix,
        multiply4x4Matrices(scalingMatrix,
                            initialTranslationMatrix));
    return transformationMatrix;
}

std::vector<float> TransformationMatrix::getZoomedMapTransformationMatrix(
    std::pair<double, double> physicalMazeSize,
    std::pair<int, int> zoomedMapPosition,
    std::pair<int, int> zoomedMapSize,
    std::pair<int, int> windowSize,
    double screenPixelsPerMeter,
    double zoomedMapScale,
    bool rotateZoomedMap,
    const Coordinate& initialMouseTranslation,
    const Coordinate& currentMouseTranslation,
    const Angle& currentMouseRotation) {

    // The purpose of this function is to produce a 4x4 matrix which,
    // when applied to the physical coordinate within the vertex shader,
    // transforms it into an OpenGL coordinate for the zoomed map. Note that
    // the zoomed map will likely not contain the entirety of the maze, so the
    // pixel coordinates will be outside of the map.

    // Step 1: Calculate the scaling matrix
    double physicalWidth = physicalMazeSize.first;
    double physicalHeight = physicalMazeSize.second;

    // Note that this is not literally the number of pixels per meter of the
    // screen. Rather, it's our desired number of pixels per simulation meter.
    double pixelsPerMeter = screenPixelsPerMeter * zoomedMapScale;
    double pixelWidth = pixelsPerMeter * physicalWidth;
    double pixelHeight = pixelsPerMeter * physicalHeight;

    std::pair<double, double> openGlOrigin =
        mapPixelCoordinateToOpenGlCoordinate(std::make_pair(0, 0), windowSize);
    std::pair<double, double> openGlMazeSize =
        mapPixelCoordinateToOpenGlCoordinate(std::make_pair(pixelWidth, pixelHeight), windowSize);

    double openGlWidth = openGlMazeSize.first - openGlOrigin.first;
    double openGlHeight = openGlMazeSize.second - openGlOrigin.second;

    double horizontalScaling = openGlWidth / physicalWidth;
    double verticalScaling = openGlHeight / physicalHeight;

    std::vector<float> scalingMatrix = {
        static_cast<float>(horizontalScaling),                                 0.0, 0.0, 0.0,
                                          0.0, static_cast<float>(verticalScaling), 0.0, 0.0,
                                          0.0,                                 0.0, 1.0, 0.0,
                                          0.0,                                 0.0, 0.0, 1.0,
    };

    // Step 2: Construct the translation matrix. We must ensure that the mouse
    // starts (static translation) and stays (dynamic translation) at the
    // center of the screen.

    // Part A: Find the static translation, i.e., the translation that puts the
    // center of the mouse (i.e., the midpoint of the line connecting its
    // wheels) in the center of the zoomed map. 
    double centerXPixels = initialMouseTranslation.getX().getMeters() * pixelsPerMeter;
    double centerYPixels = initialMouseTranslation.getY().getMeters() * pixelsPerMeter;
    double zoomedMapCenterXPixels = zoomedMapPosition.first + 0.5 * zoomedMapSize.first;
    double zoomedMapCenterYPixels = zoomedMapPosition.second + 0.5 * zoomedMapSize.second;
    double staticTranslationXPixels = zoomedMapCenterXPixels - centerXPixels;
    double staticTranslationYPixels = zoomedMapCenterYPixels - centerYPixels;
    std::pair<double, double> staticTranslation =
        mapPixelCoordinateToOpenGlCoordinate(std::make_pair(staticTranslationXPixels, staticTranslationYPixels), windowSize);

    // Part B: Find the dynamic translation, i.e., the current translation of the mouse.
    Cartesian mouseTranslationDelta = Cartesian(currentMouseTranslation) - initialMouseTranslation;
    double dynamicTranslationXPixels = mouseTranslationDelta.getX().getMeters() * pixelsPerMeter;
    double dynamicTranslationYPixels = mouseTranslationDelta.getY().getMeters() * pixelsPerMeter;
    std::pair<double, double> dynamicTranslation =
        mapPixelCoordinateToOpenGlCoordinate(std::make_pair(dynamicTranslationXPixels, dynamicTranslationYPixels), windowSize);

    // Combine the transalations and form the translation matrix
    double horizontalTranslation = staticTranslation.first -  dynamicTranslation.first + openGlOrigin.first;
    double verticalTranslation = staticTranslation.second - dynamicTranslation.second + openGlOrigin.second;
    std::vector<float> translationMatrix = {
        1.0, 0.0, 0.0, static_cast<float>(horizontalTranslation), 
        0.0, 1.0, 0.0,   static_cast<float>(verticalTranslation),
        0.0, 0.0, 1.0,                                       0.0,
        0.0, 0.0, 0.0,                                       1.0,
    };

    // Step 3: Construct a few other transformation matrices needed for
    // rotating the maze. In order to properly rotate the maze, we must first
    // translate the center of the mouse to the origin. Then we have to unscale
    // it, rotate it, scale it, and then translate it back to the proper
    // location. Hence all of the matrices.

    // We subtract Degrees(90) here since we want forward to face NORTH
    double theta = (Degrees(currentMouseRotation) - Degrees(90)).getRadiansZeroTo2pi();
    std::vector<float> rotationMatrix = {
        static_cast<float>( std::cos(theta)), static_cast<float>(std::sin(theta)), 0.0, 0.0,
        static_cast<float>(-std::sin(theta)), static_cast<float>(std::cos(theta)), 0.0, 0.0,
                                    0.0,                            0.0, 1.0, 0.0,
                                    0.0,                            0.0, 0.0, 1.0,
    };

    std::vector<float> inverseScalingMatrix = {
        static_cast<float>(1.0/horizontalScaling),                                     0.0, 0.0, 0.0,
                                              0.0, static_cast<float>(1.0/verticalScaling), 0.0, 0.0,
                                              0.0,                                     0.0, 1.0, 0.0,
                                              0.0,                                     0.0, 0.0, 1.0,
    };

    std::pair<double, double> zoomedMapCenterOpenGl =
        mapPixelCoordinateToOpenGlCoordinate(std::make_pair(zoomedMapCenterXPixels, zoomedMapCenterYPixels), windowSize);

    std::vector<float> translateToOriginMatrix = {
        1.0, 0.0, 0.0,  static_cast<float>(zoomedMapCenterOpenGl.first),
        0.0, 1.0, 0.0, static_cast<float>(zoomedMapCenterOpenGl.second),
        0.0, 0.0, 1.0,                                              0.0,
        0.0, 0.0, 0.0,                                              1.0,
    };

    std::vector<float> inverseTranslateToOriginMatrix = {
        1.0, 0.0, 0.0,  static_cast<float>(-zoomedMapCenterOpenGl.first),
        0.0, 1.0, 0.0, static_cast<float>(-zoomedMapCenterOpenGl.second),
        0.0, 0.0, 1.0,                                               0.0,
        0.0, 0.0, 0.0,                                               1.0,
    };

    std::vector<float> zoomedMapCameraMatrix = multiply4x4Matrices(translationMatrix, scalingMatrix);
    if (rotateZoomedMap) {
        zoomedMapCameraMatrix =
            multiply4x4Matrices(translateToOriginMatrix,
            multiply4x4Matrices(scalingMatrix,
            multiply4x4Matrices(rotationMatrix,
            multiply4x4Matrices(inverseScalingMatrix,
            multiply4x4Matrices(inverseTranslateToOriginMatrix,
                                zoomedMapCameraMatrix)))));
    }

    return zoomedMapCameraMatrix;
}

std::pair<double, double> TransformationMatrix::mapPixelCoordinateToOpenGlCoordinate(
        std::pair<double, double> coordinate,
        std::pair<int, int> windowSize) {
    return std::make_pair(
        2 * coordinate.first / windowSize.first - 1,
        2 * coordinate.second / windowSize.second - 1);
}

std::vector<float> TransformationMatrix::multiply4x4Matrices(
        std::vector<float> left,
        std::vector<float> right) {
    SIM_ASSERT_EQ(left.size(), 16);
    SIM_ASSERT_EQ(right.size(), 16);
    std::vector<float> result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double value = 0.0;
            for (int k = 0; k < 4; k++) {
                value += left.at(4*i+k) * right.at(4*k+j);
            }
            result.push_back(value);
        }
    }
    SIM_ASSERT_EQ(result.size(), 16);
    return result;
}

} // namespace sim
