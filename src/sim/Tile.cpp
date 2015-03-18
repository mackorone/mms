#include "Tile.h"

#include "Param.h"

namespace sim{

Tile::Tile() : m_x(-1), m_y(-1) {
    for (Direction direction : DIRECTIONS) {
        m_walls[direction] = false;
    }
}

Tile::~Tile()
{ }

int Tile::getX() const {
    return m_x;
}

int Tile::getY() const {
    return m_y;
}

void Tile::setPos(int x, int y) {
    m_x = x;
    m_y = y;
}

bool Tile::isWall(Direction direction) const {
    return m_walls.at(direction);
}

void Tile::setWall(Direction direction, bool isWall) {
    m_walls[direction] = isWall;
}

Polygon Tile::getFullPolygon() const {
    return m_fullPolygon;
}

Polygon Tile::getInteriorPolygon() const {
    return m_interiorPolygon;
}

std::vector<Polygon> Tile::getAllWallPolygons() const {
    return m_allWallPolygons;
}

std::vector<Polygon> Tile::getActualWallPolygons() const {
    return m_actualWallPolygons;
}

std::vector<Polygon> Tile::getCornerPolygons() const {
    return m_cornerPolygons;
}

void Tile::initPolygons() {

    //  The polygons associated with each tile are as follows:
    //
    //      full: 05af
    //
    //      interior: 278d
    //
    //      northWall: 7698
    //      eastWall: d8be
    //      southWall: 32dc
    //      westWall: 1472
    //
    //      lowerLeftCorner: 0123
    //      upperLeftCorner: 4567
    //      upperRightCorner: 89ab
    //      lowerRightCorner: cdef
    //
    //      5--6-------------9--a
    //      |  |             |  |
    //      4--7-------------8--b
    //      |  |             |  |
    //      |  |             |  |
    //      |  |             |  |
    //      |  |             |  |
    //      |  |             |  |
    //      1--2-------------d--e
    //      |  |             |  |
    //      0--3-------------c--f

    // Order is important
    initFullPolygon();
    initInteriorPolygon();
    initWallPolygons();
    initCornerPolygons();
}

void Tile::initFullPolygon() {
    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Cartesian lowerLeftPoint(tileLength * m_x, tileLength * m_y);
    Cartesian upperRightPoint(tileLength * (m_x + 1), tileLength * (m_y + 1));
    Cartesian lowerRightPoint(upperRightPoint.getX(), lowerLeftPoint.getY());
    Cartesian upperLeftPoint(lowerLeftPoint.getX(), upperRightPoint.getY());
    std::vector<Cartesian> fullPolygon {lowerLeftPoint, upperLeftPoint, upperRightPoint, lowerRightPoint};
    m_fullPolygon = Polygon(fullPolygon);
}

void Tile::initInteriorPolygon() {

    Cartesian lowerLeftPoint = m_fullPolygon.getVertices().at(0);
    Cartesian upperLeftPoint = m_fullPolygon.getVertices().at(1);
    Cartesian upperRightPoint = m_fullPolygon.getVertices().at(2);
    Cartesian lowerRightPoint = m_fullPolygon.getVertices().at(3);

    Meters halfWallWidth = Meters(P()->wallWidth()) / 2.0;

    std::vector<Cartesian> interiorPolygon {
        lowerLeftPoint + Cartesian(halfWallWidth, halfWallWidth),
        upperLeftPoint + Cartesian(halfWallWidth, halfWallWidth * -1),
        upperRightPoint + Cartesian(halfWallWidth * -1, halfWallWidth * -1),
        lowerRightPoint + Cartesian(halfWallWidth * -1, halfWallWidth)};

    m_interiorPolygon = Polygon(interiorPolygon);
}

void Tile::initWallPolygons() {

    Cartesian lowerLeftPoint = m_fullPolygon.getVertices().at(0);
    Cartesian upperLeftPoint = m_fullPolygon.getVertices().at(1);
    Cartesian upperRightPoint = m_fullPolygon.getVertices().at(2);
    Cartesian lowerRightPoint = m_fullPolygon.getVertices().at(3);

    Meters halfWallWidth = Meters(P()->wallWidth()) / 2.0;

    std::vector<Cartesian> northWall;
    northWall.push_back(Cartesian(upperLeftPoint.getX() + halfWallWidth, upperLeftPoint.getY() - halfWallWidth));
    northWall.push_back(Cartesian(upperLeftPoint.getX() + halfWallWidth, upperLeftPoint.getY()));
    northWall.push_back(Cartesian(upperRightPoint.getX() - halfWallWidth, upperRightPoint.getY()));
    northWall.push_back(Cartesian(upperRightPoint.getX() - halfWallWidth, upperRightPoint.getY() - halfWallWidth));
    m_allWallPolygons.insert(m_allWallPolygons.begin() + NORTH, Polygon(northWall));
    if (isWall(NORTH)) {
        m_actualWallPolygons.push_back(northWall);
    }

    std::vector<Cartesian> eastWall;
    eastWall.push_back(Cartesian(lowerRightPoint.getX() - halfWallWidth, lowerRightPoint.getY() + halfWallWidth));
    eastWall.push_back(Cartesian(upperRightPoint.getX() - halfWallWidth, upperRightPoint.getY() - halfWallWidth));
    eastWall.push_back(Cartesian(upperRightPoint.getX(), upperRightPoint.getY() - halfWallWidth));
    eastWall.push_back(Cartesian(lowerRightPoint.getX(), lowerRightPoint.getY() + halfWallWidth));
    m_allWallPolygons.insert(m_allWallPolygons.begin() + EAST, Polygon(eastWall));
    if (isWall(EAST)) {
        m_actualWallPolygons.push_back(eastWall);
    }

    std::vector<Cartesian> southWall;
    southWall.push_back(Cartesian(lowerLeftPoint.getX() + halfWallWidth, lowerLeftPoint.getY()));
    southWall.push_back(Cartesian(lowerLeftPoint.getX() + halfWallWidth, lowerLeftPoint.getY() + halfWallWidth));
    southWall.push_back(Cartesian(lowerRightPoint.getX() - halfWallWidth, lowerRightPoint.getY() + halfWallWidth));
    southWall.push_back(Cartesian(lowerRightPoint.getX() - halfWallWidth, lowerRightPoint.getY()));
    m_allWallPolygons.insert(m_allWallPolygons.begin() + SOUTH, Polygon(southWall));
    if (isWall(SOUTH)) {
        m_actualWallPolygons.push_back(southWall);
    }


    std::vector<Cartesian> westWall;
    westWall.push_back(Cartesian(lowerLeftPoint.getX(), lowerLeftPoint.getY() + halfWallWidth));
    westWall.push_back(Cartesian(upperLeftPoint.getX(), upperLeftPoint.getY() - halfWallWidth));
    westWall.push_back(Cartesian(upperLeftPoint.getX() + halfWallWidth, upperLeftPoint.getY() - halfWallWidth));
    westWall.push_back(Cartesian(lowerLeftPoint.getX() + halfWallWidth, lowerLeftPoint.getY() + halfWallWidth));
    m_allWallPolygons.insert(m_allWallPolygons.begin() + WEST, Polygon(westWall));
    if (isWall(WEST)) {
        m_actualWallPolygons.push_back(westWall);
    }

}

void Tile::initCornerPolygons() {

    Cartesian lowerLeftPoint = m_fullPolygon.getVertices().at(0);
    Cartesian upperLeftPoint = m_fullPolygon.getVertices().at(1);
    Cartesian upperRightPoint = m_fullPolygon.getVertices().at(2);
    Cartesian lowerRightPoint = m_fullPolygon.getVertices().at(3);

    Meters halfWallWidth = Meters(P()->wallWidth()) / 2.0;

    std::vector<Cartesian> lowerLeftCorner;
    lowerLeftCorner.push_back(Cartesian(lowerLeftPoint.getX(), lowerLeftPoint.getY()));
    lowerLeftCorner.push_back(Cartesian(lowerLeftPoint.getX(), lowerLeftPoint.getY() + halfWallWidth));
    lowerLeftCorner.push_back(Cartesian(lowerLeftPoint.getX() + halfWallWidth, lowerLeftPoint.getY() + halfWallWidth));
    lowerLeftCorner.push_back(Cartesian(lowerLeftPoint.getX() + halfWallWidth, lowerLeftPoint.getY()));
    m_cornerPolygons.push_back(Polygon(lowerLeftCorner));

    std::vector<Cartesian> upperLeftCorner;
    upperLeftCorner.push_back(Cartesian(upperLeftPoint.getX(), upperLeftPoint.getY()));
    upperLeftCorner.push_back(Cartesian(upperLeftPoint.getX() + halfWallWidth, upperLeftPoint.getY()));
    upperLeftCorner.push_back(Cartesian(upperLeftPoint.getX() + halfWallWidth, upperLeftPoint.getY() - halfWallWidth));
    upperLeftCorner.push_back(Cartesian(upperLeftPoint.getX(), upperLeftPoint.getY() - halfWallWidth));
    m_cornerPolygons.push_back(Polygon(upperLeftCorner));

    std::vector<Cartesian> upperRightCorner;
    upperRightCorner.push_back(Cartesian(upperRightPoint.getX(), upperRightPoint.getY()));
    upperRightCorner.push_back(Cartesian(upperRightPoint.getX(), upperRightPoint.getY() - halfWallWidth));
    upperRightCorner.push_back(Cartesian(upperRightPoint.getX() - halfWallWidth, upperRightPoint.getY() - halfWallWidth));
    upperRightCorner.push_back(Cartesian(upperRightPoint.getX() - halfWallWidth, upperRightPoint.getY()));
    m_cornerPolygons.push_back(Polygon(upperRightCorner));

    std::vector<Cartesian> lowerRightCorner;
    lowerRightCorner.push_back(Cartesian(lowerRightPoint.getX(), lowerRightPoint.getY()));
    lowerRightCorner.push_back(Cartesian(lowerRightPoint.getX() - halfWallWidth, lowerRightPoint.getY()));
    lowerRightCorner.push_back(Cartesian(lowerRightPoint.getX() - halfWallWidth, lowerRightPoint.getY() + halfWallWidth));
    lowerRightCorner.push_back(Cartesian(lowerRightPoint.getX(), lowerRightPoint.getY() + halfWallWidth));
    m_cornerPolygons.push_back(Polygon(lowerRightCorner));
}

} // namespace sim
