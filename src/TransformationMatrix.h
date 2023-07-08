#pragma once

#include <QMatrix4x4>
#include <QPair>

namespace mms {

class TransformationMatrix {
 public:
  TransformationMatrix() = delete;

  // Get a 4x4 matrix which, when applied to a physical coordinate in the
  // vertex shader, transforms it into an OpenGL coordinate for the map
  static QMatrix4x4 get(int mazeWidth, int mazeHeight, int mapWidthPixels,
                        int mapHeightPixels);

 private:
  // Translate from a pixel coordinate to an OpenGL coordinate
  // Pixel coordinate: LL is (0, 0), UR is (width, height)
  // OpenGL coordinate: LL is (-1, -1), UR is (1, 1)
  static QPair<double, double> pixelToOpenGl(QPair<double, double> coordinate,
                                             QPair<int, int> windowSize);

  // Multiplies two 4x4 matrices together
  static QVector<double> multiply4x4Matrices(QVector<double> left,
                                             QVector<double> right);
};

}  // namespace mms
