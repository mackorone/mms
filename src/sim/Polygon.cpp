#include "Polygon.h"

#include <polypartition/polypartition.h>

#include "Assert.h"
#include "units/Polar.h"
#include "CPMath.h"

namespace sim {

Polygon::Polygon() {
}

Polygon::Polygon(const Polygon& polygon) {
    m_vertices = polygon.getVertices();
}

Polygon::Polygon(const std::vector<Cartesian>& vertices) {
    ASSERT_LE(3, vertices.size());
    m_vertices = vertices;
}

Polygon Polygon::createCirclePolygon(const Cartesian& position, const Distance& radius, int numberOfEdges) {
    ASSERT_LE(3, numberOfEdges);
    std::vector<Cartesian> polygon;
    for (int i = 0; i < numberOfEdges; i += 1) {
        polygon.push_back(Polar(radius, Radians(i * M_TWOPI / numberOfEdges)) + position);
    }
    return Polygon(polygon);
}

std::vector<Cartesian> Polygon::getVertices() const {
    return m_vertices;
}

std::vector<std::pair<Cartesian, Cartesian>> Polygon::getLineSegments() const {
    std::vector<std::pair<Cartesian, Cartesian>> segments;
    if (2 < m_vertices.size()) {
        Cartesian previousPoint = m_vertices.back();
        for (Cartesian currentPoint : m_vertices) {
            segments.push_back(std::make_pair(previousPoint, currentPoint));
            previousPoint = currentPoint;
        }
    }
    return segments;
}

MetersSquared Polygon::area() const {

    // See http://mathworld.wolfram.com/PolygonArea.html

    double area = 0.0;
    for (int i = 0; i < m_vertices.size(); i += 1) {
        int j = (i + 1) % m_vertices.size();
        area += m_vertices.at(i).getX().getMeters() * m_vertices.at(j).getY().getMeters();
        area -= m_vertices.at(i).getY().getMeters() * m_vertices.at(j).getX().getMeters();
    }
    area /= 2.0;
    return MetersSquared(std::abs(area));
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
        vertices.push_back(vertex + translation);
    }
    return Polygon(vertices);
}

Polygon Polygon::rotateAroundPoint(const Angle& angle, const Coordinate& point) const {
    std::vector<Cartesian> vertices;
    for (Cartesian vertex : m_vertices) {
        Cartesian relative(vertex.getX() - point.getX(), vertex.getY() - point.getY());
        Polar rotated(relative.getRho(), relative.getTheta() + angle);
        Cartesian absolute(rotated.getX() + point.getX(), rotated.getY() + point.getY());
        vertices.push_back(absolute);
    }
    return Polygon(vertices);
}

std::vector<Triangle> Polygon::triangulate() const {

    // Populate the TPPLPoly
    TPPLPoly tpplPoly;
    tpplPoly.Init(m_vertices.size());
    for (int i = 0; i < m_vertices.size(); i += 1) {
        tpplPoly[i].x = m_vertices.at(i).getX().getMeters();
        tpplPoly[i].y = m_vertices.at(i).getY().getMeters();
    }
    tpplPoly.SetOrientation(TPPL_CCW);

    // Perform the triangulation
    TPPLPartition triangulator;
    std::list<TPPLPoly> result;
    triangulator.Triangulate_OPT(&tpplPoly, &result);

    // Populate the output vector
    std::vector<Triangle> triangles;
    for (auto it = result.begin(); it != result.end(); it++) {
        Triangle triangle {
            Cartesian(Meters((*it)[0].x), Meters((*it)[0].y)),
            Cartesian(Meters((*it)[1].x), Meters((*it)[1].y)),
            Cartesian(Meters((*it)[2].x), Meters((*it)[2].y)),
        };
        triangles.push_back(triangle);
    }

    return triangles;
}

} // namespace sim
