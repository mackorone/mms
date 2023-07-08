#pragma once

#include <QVector>

#include "Triangle.h"
#include "units/Angle.h"
#include "units/Coordinate.h"

namespace mms {

class Polygon {
 public:
  Polygon();
  Polygon(const Polygon &polygon);
  Polygon(QVector<Coordinate> vertices);

  QVector<Coordinate> getVertices() const;
  QVector<Triangle> getTriangles() const;

  Polygon translate(const Coordinate &translation) const;
  Polygon rotateAroundPoint(const Angle &angle, const Coordinate &point) const;

 private:
  QVector<Coordinate> m_vertices;

  // We're lazy about triangulation, since it's expensive and not always
  // necessary. The "mutable" keyword allows us to assign m_triangles in the
  // const function getTriangles().
  mutable QVector<Triangle> m_triangles;

  // This special constructor makes it so that rotate and translate don't
  // require re-triangulation. We keep it private since it's pretty easy to
  // abuse the fact that the triangles argument should be the triangulation
  // of the polygon specified by the vertices argument.
  Polygon(QVector<Coordinate> vertices, QVector<Triangle> triangles);

  // Tells us whether or not the polygon has already performed triangulation.
  // This is used in the copy constructor, and allows us to be lazy without
  // throwing away information.
  bool alreadyPerformedTriangulation() const;

  // Actually peforms the triangulation of the polygon.
  static QVector<Triangle> triangulate(QVector<Coordinate> vertices);
};

}  // namespace mms
