#include "Polygon.h"

#include <polypartition/polypartition.h>

#include "Assert.h"
#include "CPMath.h"
#include "GeometryUtilities.h"
#include "Logging.h"
#include "SimUtilities.h"
#include "units/Polar.h"

namespace sim {

Polygon::Polygon() {
}

Polygon::Polygon(const Polygon& polygon) :
    m_vertices(polygon.getVertices()) {
    // If the polygon being copyed has already been triangulated, we should
    // grab the triangles, lest we have to re-triangulate in the future. If not,
    // we can be lazy, in case the triangles for this polygon aren't needed.
    if (polygon.alreadyPerformedTriangulation()) {
        m_triangles = polygon.getTriangles();
    }
    else {
        // Uncomment to log untriangulated polygon copies
        /*
        static int i = 0;
        i += 1;
        if (i % 100 == 0) {
            L()->info("Untriangulated polygon copies count: %v", i);
        }
        */
    }
}

Polygon::Polygon(const std::vector<Cartesian>& vertices) :
    m_vertices(vertices),
    // Postpone triangulation until we absolutely have to do it.
    m_triangles({}) {
    SIM_ASSERT_LE(3, m_vertices.size());
    // If the number of vertices is three, the triangulation is trivial
    if (m_vertices.size() == 3) {
        m_triangles = {{
            m_vertices.at(0),
            m_vertices.at(1),
            m_vertices.at(2),
        }};
    }
}

std::vector<Cartesian> Polygon::getVertices() const {
    return m_vertices;
}

std::vector<Triangle> Polygon::getTriangles() const {
    // Lazy initialization here
    if (m_triangles.size() == 0) {
        m_triangles = triangulate(m_vertices);
    }
    return m_triangles;
}

MetersSquared Polygon::area() const {

    // See http://mathworld.wolfram.com/PolygonArea.html

    double sumOfDeterminants = 0.0;
    for (int i = 0; i < m_vertices.size(); i += 1) {
        int j = (i + 1) % m_vertices.size();
        sumOfDeterminants += m_vertices.at(i).getX().getMeters() * m_vertices.at(j).getY().getMeters();
        sumOfDeterminants -= m_vertices.at(i).getY().getMeters() * m_vertices.at(j).getX().getMeters();
    }
    return MetersSquared(std::abs(sumOfDeterminants / 2.0));
}

Cartesian Polygon::centroid() const {

    // See http://en.wikipedia.org/wiki/Centroid#Centroid_of_polygon

    Meters cx(0.0);
    Meters cy(0.0);
    for (int i = 0; i < m_vertices.size(); i += 1) {
        int j = (i + 1) % m_vertices.size();
        cx += (m_vertices.at(i).getX() + m_vertices.at(j).getX())
            * (m_vertices.at(i).getX() * m_vertices.at(j).getY() - m_vertices.at(j).getX() * m_vertices.at(i).getY()).getMetersSquared();
        cy += (m_vertices.at(i).getY() + m_vertices.at(j).getY())
            * (m_vertices.at(i).getX() * m_vertices.at(j).getY() - m_vertices.at(j).getX() * m_vertices.at(i).getY()).getMetersSquared();
    }
    cx = Meters(std::abs(cx.getMeters()));
    cy = Meters(std::abs(cy.getMeters()));
    return Cartesian(cx / (area() * 6).getMetersSquared(), cy / (area() * 6).getMetersSquared());
}

Polygon Polygon::translate(const Coordinate& translation) const {

    std::vector<Cartesian> vertices;
    for (Cartesian vertex : m_vertices) {
        vertices.push_back(GeometryUtilities::translateVertex(vertex, translation));
    }

    std::vector<Triangle> triangles;
    for (Triangle triangle : m_triangles) {
        triangles.push_back({
            GeometryUtilities::translateVertex(triangle.p1, translation),
            GeometryUtilities::translateVertex(triangle.p2, translation),
            GeometryUtilities::translateVertex(triangle.p3, translation),
        });
    }

    return Polygon(vertices, triangles);
}

Polygon Polygon::rotateAroundPoint(const Angle& angle, const Coordinate& point) const {

    std::vector<Cartesian> vertices;
    for (Cartesian vertex : m_vertices) {
        vertices.push_back(GeometryUtilities::rotateVertexAroundPoint(vertex, angle, point));
    }

    std::vector<Triangle> triangles;
    for (Triangle triangle : m_triangles) {
        triangles.push_back({
            GeometryUtilities::rotateVertexAroundPoint(triangle.p1, angle, point),
            GeometryUtilities::rotateVertexAroundPoint(triangle.p2, angle, point),
            GeometryUtilities::rotateVertexAroundPoint(triangle.p3, angle, point),
        });
    }

    return Polygon(vertices, triangles);
}

Polygon::Polygon(const std::vector<Cartesian>& vertices, const std::vector<Triangle>& triangles) :
    m_vertices(vertices),
    m_triangles(triangles) {
}

bool Polygon::alreadyPerformedTriangulation() const {
    return 0 < m_triangles.size();
}

std::vector<Triangle> Polygon::triangulate(const std::vector<Cartesian>& vertices) {

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
    std::vector<Triangle> triangles;
    for (auto it = result.begin(); it != result.end(); it++) {
        triangles.push_back({
            Cartesian(Meters((*it)[0].x), Meters((*it)[0].y)),
            Cartesian(Meters((*it)[1].x), Meters((*it)[1].y)),
            Cartesian(Meters((*it)[2].x), Meters((*it)[2].y)),
        });
    }

    return triangles;
}

} // namespace sim
