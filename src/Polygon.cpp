#include "Polygon.h"

#include <QtMath>

#include "AssertMacros.h"
#include "GeometryUtilities.h"
#include "SimUtilities.h"
#include "polypartition/polypartition.h"

namespace mms {

Polygon::Polygon() {}

Polygon::Polygon(const Polygon &polygon) : m_vertices(polygon.getVertices()) {
  // If the polygon being copyed has already been triangulated, we should
  // grab the triangles, lest we have to re-triangulate in the future. If not,
  // we can be lazy, in case the triangles for this polygon aren't needed.
  if (polygon.alreadyPerformedTriangulation()) {
    m_triangles = polygon.getTriangles();
  } else {
    // Uncomment to log untriangulated polygon copies
    /*
    static int i = 0;
    i += 1;
    if (i % 100 == 0) {
        qInfo().noquote().nospace()
            << "Untriangulated polygon copies count: %v", i);
    }
    */
  }
}

Polygon::Polygon(QVector<Coordinate> vertices)
    : m_vertices(vertices),
      // Postpone triangulation until we absolutely have to do it.
      m_triangles({}) {
  ASSERT_LE(3, m_vertices.size());
  // If the number of vertices is three, the triangulation is trivial
  if (m_vertices.size() == 3) {
    m_triangles = {{
        m_vertices.at(0),
        m_vertices.at(1),
        m_vertices.at(2),
    }};
  }
}

QVector<Coordinate> Polygon::getVertices() const { return m_vertices; }

QVector<Triangle> Polygon::getTriangles() const {
  // Lazy initialization here
  if (m_triangles.size() == 0) {
    m_triangles = triangulate(m_vertices);
  }
  return m_triangles;
}

Polygon Polygon::translate(const Coordinate &translation) const {
  QVector<Coordinate> vertices;
  for (const Coordinate &vertex : m_vertices) {
    vertices.append(GeometryUtilities::translateVertex(vertex, translation));
  }

  QVector<Triangle> triangles;
  for (const Triangle &triangle : m_triangles) {
    triangles.append({
        GeometryUtilities::translateVertex(triangle.p1, translation),
        GeometryUtilities::translateVertex(triangle.p2, translation),
        GeometryUtilities::translateVertex(triangle.p3, translation),
    });
  }

  return Polygon(vertices, triangles);
}

Polygon Polygon::rotateAroundPoint(const Angle &angle,
                                   const Coordinate &point) const {
  QVector<Coordinate> vertices;
  for (const Coordinate &vertex : m_vertices) {
    vertices.append(
        GeometryUtilities::rotateVertexAroundPoint(vertex, point, angle));
  }

  QVector<Triangle> triangles;
  for (const Triangle &triangle : m_triangles) {
    triangles.append({
        GeometryUtilities::rotateVertexAroundPoint(triangle.p1, point, angle),
        GeometryUtilities::rotateVertexAroundPoint(triangle.p2, point, angle),
        GeometryUtilities::rotateVertexAroundPoint(triangle.p3, point, angle),
    });
  }

  return Polygon(vertices, triangles);
}

Polygon::Polygon(QVector<Coordinate> vertices, QVector<Triangle> triangles)
    : m_vertices(vertices), m_triangles(triangles) {}

bool Polygon::alreadyPerformedTriangulation() const {
  return 0 < m_triangles.size();
}

QVector<Triangle> Polygon::triangulate(QVector<Coordinate> vertices) {
  // Populate the TPPLPoly
  TPPLPoly tpplPoly;
  tpplPoly.Init(vertices.size());
  for (int i = 0; i < vertices.size(); i += 1) {
    tpplPoly[i].x = vertices.at(i).getX().getMeters();
    tpplPoly[i].y = vertices.at(i).getY().getMeters();
  }
  tpplPoly.SetOrientation(TPPL_CCW);

  // Perform the triangulation
  TPPLPartition triangulator;
  std::list<TPPLPoly> result;
  triangulator.Triangulate_EC(&tpplPoly, &result);

  // Populate the output vector
  QVector<Triangle> triangles;
  for (auto it = result.begin(); it != result.end(); it++) {
    triangles.append({
        Coordinate::Cartesian(Distance::Meters((*it)[0].x),
                              Distance::Meters((*it)[0].y)),
        Coordinate::Cartesian(Distance::Meters((*it)[1].x),
                              Distance::Meters((*it)[1].y)),
        Coordinate::Cartesian(Distance::Meters((*it)[2].x),
                              Distance::Meters((*it)[2].y)),
    });
  }

  return triangles;
}

}  // namespace mms
