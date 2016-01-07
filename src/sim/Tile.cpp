#include "Tile.h"

#include "Param.h"

namespace sim{

Tile::Tile() : m_x(-1), m_y(-1), m_distance(-1) {
    for (Direction direction : DIRECTIONS) {
        m_walls.insert(std::make_pair(direction, false));
    }
}

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
    m_walls.at(direction) = isWall;
}

int Tile::getDistance() const {
    return m_distance;
}

void Tile::setDistance(int distance) {
    m_distance= distance;
}

Polygon Tile::getFullPolygon() const {
    return m_fullPolygon;
}

Polygon Tile::getInteriorPolygon() const {
    return m_interiorPolygon;
}

Polygon Tile::getWallPolygon(Direction direction) const {
    return m_wallPolygons.at(direction);
}

std::vector<Polygon> Tile::getCornerPolygons() const {
    return m_cornerPolygons;
}

void Tile::initPolygons(int mazeWidth, int mazeHeight) {

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
    //      5---6-------------9---a
    //      |   |             |   |
    //      4---7-------------8---b
    //      |   |             |   |
    //      |   |             |   |
    //      |   |             |   |
    //      |   |             |   |
    //      |   |             |   |
    //      1---2-------------d---e
    //      |   |             |   |
    //      0---3-------------c---f

    // Order is important
    initFullPolygon(mazeWidth, mazeHeight);
    initInteriorPolygon(mazeWidth, mazeHeight);
    initWallPolygons(mazeWidth, mazeHeight);
    initCornerPolygons(mazeWidth, mazeHeight);
}


void Tile::initFullPolygon(int mazeWidth, int mazeHeight) {

    Meters halfWallWidth = Meters(P()->wallWidth()) / 2.0;
    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Cartesian lowerLeftPoint(tileLength * getX() - halfWallWidth * (getX() == 0 ? 1 : 0),
                             tileLength * getY() - halfWallWidth * (getY() == 0 ? 1 : 0));
    Cartesian upperRightPoint(tileLength * (getX() + 1) + halfWallWidth * (getX() == mazeWidth - 1 ? 1 : 0),
                              tileLength * (getY() + 1) + halfWallWidth * (getY() == mazeHeight - 1 ? 1 : 0));
    Cartesian lowerRightPoint(upperRightPoint.getX(), lowerLeftPoint.getY());
    Cartesian upperLeftPoint(lowerLeftPoint.getX(), upperRightPoint.getY());
    m_fullPolygon = Polygon({lowerLeftPoint, upperLeftPoint, upperRightPoint, lowerRightPoint});
}

void Tile::initInteriorPolygon(int mazeWidth, int mazeHeight) {

    Meters halfWallWidth = Meters(P()->wallWidth()) / 2.0;
    Cartesian lowerLeftPoint = m_fullPolygon.getVertices().at(0);
    Cartesian upperLeftPoint = m_fullPolygon.getVertices().at(1);
    Cartesian upperRightPoint = m_fullPolygon.getVertices().at(2);
    Cartesian lowerRightPoint = m_fullPolygon.getVertices().at(3);

    m_interiorPolygon = Polygon({
        lowerLeftPoint + Cartesian(
            halfWallWidth * (getX() == 0 ? 2 : 1), halfWallWidth * (getY() == 0 ? 2 : 1)),
        upperLeftPoint + Cartesian(
            halfWallWidth * (getX() == 0 ? 2 : 1), halfWallWidth * (getY() == mazeHeight - 1 ? -2 : -1)),
        upperRightPoint + Cartesian(
            halfWallWidth * (getX() == mazeWidth - 1 ? -2 : -1), halfWallWidth * (getY() == mazeHeight - 1 ? -2 : -1)),
        lowerRightPoint + Cartesian(
            halfWallWidth * (getX() == mazeWidth - 1 ? -2 : -1), halfWallWidth * (getY() == 0 ? 2 : 1))});
}

void Tile::initWallPolygons(int mazeWidth, int mazeHeight) {

    Cartesian outerLowerLeftPoint = m_fullPolygon.getVertices().at(0);
    Cartesian outerUpperLeftPoint = m_fullPolygon.getVertices().at(1);
    Cartesian outerUpperRightPoint = m_fullPolygon.getVertices().at(2);
    Cartesian outerLowerRightPoint = m_fullPolygon.getVertices().at(3);

    Cartesian innerLowerLeftPoint = m_interiorPolygon.getVertices().at(0);
    Cartesian innerUpperLeftPoint = m_interiorPolygon.getVertices().at(1);
    Cartesian innerUpperRightPoint = m_interiorPolygon.getVertices().at(2);
    Cartesian innerLowerRightPoint = m_interiorPolygon.getVertices().at(3);

    std::vector<Cartesian> northWall;
    northWall.push_back(innerUpperLeftPoint);
    northWall.push_back(Cartesian(innerUpperLeftPoint.getX(), outerUpperLeftPoint.getY()));
    northWall.push_back(Cartesian(innerUpperRightPoint.getX(), outerUpperRightPoint.getY()));
    northWall.push_back(innerUpperRightPoint);
    m_wallPolygons.insert(std::make_pair(Direction::NORTH, Polygon(northWall)));

    std::vector<Cartesian> eastWall;
    eastWall.push_back(innerLowerRightPoint);
    eastWall.push_back(innerUpperRightPoint);
    eastWall.push_back(Cartesian(outerUpperRightPoint.getX(), innerUpperRightPoint.getY()));
    eastWall.push_back(Cartesian(outerLowerRightPoint.getX(), innerLowerRightPoint.getY()));
    m_wallPolygons.insert(std::make_pair(Direction::EAST, Polygon(eastWall)));

    std::vector<Cartesian> southWall;
    southWall.push_back(Cartesian(innerLowerLeftPoint.getX(), outerLowerLeftPoint.getY()));
    southWall.push_back(innerLowerLeftPoint);
    southWall.push_back(innerLowerRightPoint);
    southWall.push_back(Cartesian(innerLowerRightPoint.getX(), outerLowerRightPoint.getY()));
    m_wallPolygons.insert(std::make_pair(Direction::SOUTH, Polygon(southWall)));

    std::vector<Cartesian> westWall;
    westWall.push_back(Cartesian(outerLowerLeftPoint.getX(), innerLowerLeftPoint.getY()));
    westWall.push_back(Cartesian(outerUpperLeftPoint.getX(), innerUpperLeftPoint.getY()));
    westWall.push_back(innerUpperLeftPoint);
    westWall.push_back(innerLowerLeftPoint);
    m_wallPolygons.insert(std::make_pair(Direction::WEST, Polygon(westWall)));
}

void Tile::initCornerPolygons(int mazeWidth, int mazeHeight) {

    Cartesian outerLowerLeftPoint = m_fullPolygon.getVertices().at(0);
    Cartesian outerUpperLeftPoint = m_fullPolygon.getVertices().at(1);
    Cartesian outerUpperRightPoint = m_fullPolygon.getVertices().at(2);
    Cartesian outerLowerRightPoint = m_fullPolygon.getVertices().at(3);

    Cartesian innerLowerLeftPoint = m_interiorPolygon.getVertices().at(0);
    Cartesian innerUpperLeftPoint = m_interiorPolygon.getVertices().at(1);
    Cartesian innerUpperRightPoint = m_interiorPolygon.getVertices().at(2);
    Cartesian innerLowerRightPoint = m_interiorPolygon.getVertices().at(3);

    std::vector<Cartesian> lowerLeftCorner;
    lowerLeftCorner.push_back(outerLowerLeftPoint);
    lowerLeftCorner.push_back(Cartesian(outerLowerLeftPoint.getX(), innerLowerLeftPoint.getY()));
    lowerLeftCorner.push_back(innerLowerLeftPoint);
    lowerLeftCorner.push_back(Cartesian(innerLowerLeftPoint.getX(), outerLowerLeftPoint.getY()));
    m_cornerPolygons.push_back(Polygon(lowerLeftCorner));

    std::vector<Cartesian> upperLeftCorner;
    upperLeftCorner.push_back(Cartesian(outerUpperLeftPoint.getX(), innerUpperLeftPoint.getY()));
    upperLeftCorner.push_back(outerUpperLeftPoint);
    upperLeftCorner.push_back(Cartesian(innerUpperLeftPoint.getX(), outerUpperLeftPoint.getY()));
    upperLeftCorner.push_back(innerUpperLeftPoint);
    m_cornerPolygons.push_back(Polygon(upperLeftCorner));

    std::vector<Cartesian> upperRightCorner;
    upperRightCorner.push_back(innerUpperRightPoint);
    upperRightCorner.push_back(Cartesian(innerUpperRightPoint.getX(), outerUpperRightPoint.getY()));
    upperRightCorner.push_back(outerUpperRightPoint);
    upperRightCorner.push_back(Cartesian(outerUpperRightPoint.getX(), innerUpperRightPoint.getY()));
    m_cornerPolygons.push_back(Polygon(upperRightCorner));

    std::vector<Cartesian> lowerRightCorner;
    lowerRightCorner.push_back(Cartesian(innerLowerRightPoint.getX(), outerLowerRightPoint.getY()));
    lowerRightCorner.push_back(innerLowerRightPoint);
    lowerRightCorner.push_back(Cartesian(outerLowerRightPoint.getX(), innerLowerRightPoint.getY()));
    lowerRightCorner.push_back(outerLowerRightPoint);
    m_cornerPolygons.push_back(Polygon(lowerRightCorner));
}

} // namespace sim
