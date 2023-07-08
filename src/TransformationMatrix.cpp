#include "TransformationMatrix.h"

#include <algorithm>

#include "AssertMacros.h"
#include "Dimensions.h"

namespace mms {

QMatrix4x4 TransformationMatrix::get(int mazeWidth, int mazeHeight,
                                     int mapWidthPixels, int mapHeightPixels) {
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

  double wallWidth = Dimensions::wallWidth().getMeters();
  QVector<double> initialTranslationMatrix = {
      1.0, 0.0, 0.0, static_cast<double>(0.5 * wallWidth),
      0.0, 1.0, 0.0, static_cast<double>(0.5 * wallWidth),
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0,
  };

  QPair<int, int> windowSize = {mapWidthPixels, mapHeightPixels};
  QPair<int, int> fullMapPosition = {5, 5};
  QPair<int, int> fullMapSize = {windowSize.first - 10, windowSize.second - 10};
  QPair<double, double> physicalMazeSize = {
      (Dimensions::wallWidth() + Dimensions::tileLength() * mazeWidth)
          .getMeters(),
      (Dimensions::wallWidth() + Dimensions::tileLength() * mazeHeight)
          .getMeters(),
  };

  // Ensure that the maze width and height always appear equally scaled. Note
  // that this is not literally the number of pixels per meter of the screen.
  // Rather, it's our desired number of pixels per simulation meter.
  double physicalWidth = physicalMazeSize.first;
  double physicalHeight = physicalMazeSize.second;
  double pixelsPerMeter = std::min(fullMapSize.first / physicalWidth,
                                   fullMapSize.second / physicalHeight);
  double pixelWidth = pixelsPerMeter * physicalWidth;
  double pixelHeight = pixelsPerMeter * physicalHeight;

  QPair<double, double> openGlOrigin = pixelToOpenGl({0, 0}, windowSize);
  QPair<double, double> openGlMazeSize =
      pixelToOpenGl({pixelWidth, pixelHeight}, windowSize);

  double openGlWidth = openGlMazeSize.first - openGlOrigin.first;
  double openGlHeight = openGlMazeSize.second - openGlOrigin.second;

  double horizontalScaling = openGlWidth / physicalWidth;
  double verticalScaling = openGlHeight / physicalHeight;

  QVector<double> scalingMatrix = {
      static_cast<double>(horizontalScaling),
      0.0,
      0.0,
      0.0,
      0.0,
      static_cast<double>(verticalScaling),
      0.0,
      0.0,
      0.0,
      0.0,
      1.0,
      0.0,
      0.0,
      0.0,
      0.0,
      1.0,
  };

  // Step 3: Construct the translation matrix. Note that here we ensure that
  // the maze is centered within the map boundaries.
  double pixelLowerLeftCornerX =
      fullMapPosition.first + 0.5 * (fullMapSize.first - pixelWidth);
  double pixelLowerLeftCornerY =
      fullMapPosition.second + 0.5 * (fullMapSize.second - pixelHeight);
  QPair<double, double> openGlLowerLeftCorner =
      pixelToOpenGl({pixelLowerLeftCornerX, pixelLowerLeftCornerY}, windowSize);
  QVector<double> translationMatrix = {
      1.0, 0.0, 0.0, static_cast<double>(openGlLowerLeftCorner.first),
      0.0, 1.0, 0.0, static_cast<double>(openGlLowerLeftCorner.second),
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0,
  };

  // Step 4: Compose the matrices
  QVector<double> m = multiply4x4Matrices(
      translationMatrix,
      multiply4x4Matrices(scalingMatrix, initialTranslationMatrix));
  return QMatrix4x4(m.at(0), m.at(1), m.at(2), m.at(3), m.at(4), m.at(5),
                    m.at(6), m.at(7), m.at(8), m.at(9), m.at(10), m.at(11),
                    m.at(12), m.at(13), m.at(14), m.at(15));
}

QPair<double, double> TransformationMatrix::pixelToOpenGl(
    QPair<double, double> coordinate, QPair<int, int> windowSize) {
  return {2 * coordinate.first / windowSize.first - 1,
          2 * coordinate.second / windowSize.second - 1};
}

QVector<double> TransformationMatrix::multiply4x4Matrices(
    QVector<double> left, QVector<double> right) {
  ASSERT_EQ(left.size(), 16);
  ASSERT_EQ(right.size(), 16);
  QVector<double> result;
  for (int i = 0; i < 4; i += 1) {
    for (int j = 0; j < 4; j++) {
      double value = 0.0;
      for (int k = 0; k < 4; k++) {
        value += left.at(4 * i + k) * right.at(4 * k + j);
      }
      result.append(value);
    }
  }
  ASSERT_EQ(result.size(), 16);
  return result;
}

}  // namespace mms
