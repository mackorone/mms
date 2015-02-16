#include "Tile.h"

#include "Param.h"

namespace sim{

Tile::Tile() : m_x(-1), m_y(-1), m_distance(P()->mazeWidth() * P()->mazeHeight()), m_explored(false),
        m_passes(0), m_posp(false), m_neighbors(), m_polygonsInitialized(false) {
    m_walls[0] = false;
    m_walls[1] = false;
    m_walls[2] = false;
    m_walls[3] = false;
}

Tile::~Tile()
{ }

int Tile::getX() {
    return m_x;
}

int Tile::getY() {
    return m_y;
}

int Tile::getDistance() {
    return m_distance;
}

int Tile::getPasses() {
    return m_passes;
}

bool Tile::getExplored() {
    return m_explored;
}

bool Tile::getPosp() {
    return m_posp;
}

bool Tile::isWall(Direction direction) {
    return m_walls[direction];
}

std::vector<Tile*> Tile::getNeighbors() {
    return m_neighbors;
}

void Tile::setPos(int x, int y) {
    m_x = x;
    m_y = y;
}

void Tile::setDistance(int distance) {
    m_distance = distance;
}

void Tile::incrementPasses() {
    m_passes++;
}

void Tile::resetPasses() {
    m_passes = 0;
}

void Tile::setExplored(bool explored) {
    m_explored = explored;
}

void Tile::setPosp(bool posp) {
    m_posp = posp;
}

void Tile::setWall(int wall, bool exists) {
    m_walls[wall] = exists;
}

void Tile::addNeighbor(Tile* neighbor) {
    m_neighbors.push_back(neighbor);
}

void Tile::resetNeighbors() {
    m_neighbors.clear();
}

Polygon Tile::getBasePolygon() {
    if (!m_polygonsInitialized) {
        initPolygons();
    }
    return m_basePolygon;
}

std::vector<Polygon> Tile::getWallPolygons() {
    if (!m_polygonsInitialized) {
        initPolygons();
    }
    return m_wallPolygons;
}

std::vector<Polygon> Tile::getCornerPolygons() {
    if (!m_polygonsInitialized) {
        initPolygons();
    }
    return m_cornerPolygons;
}

void Tile::initPolygons() {
    // Order is important
    initBasePolygon();
    initWallPolygons();
    initCornerPolygons();
    m_polygonsInitialized = true;
}

void Tile::initBasePolygon() {
    Meters tileLength = Meters(P()->wallLength() + P()->wallWidth());
    Cartesian lowerLeftPoint(tileLength * m_x, tileLength * m_y);
    Cartesian upperRightPoint(tileLength * (m_x + 1), tileLength * (m_y + 1));
    Cartesian lowerRightPoint(upperRightPoint.getX(), lowerLeftPoint.getY());
    Cartesian upperLeftPoint(lowerLeftPoint.getX(), upperRightPoint.getY());
    std::vector<Cartesian> basePolygon {lowerLeftPoint, upperLeftPoint, upperRightPoint, lowerRightPoint};
    m_basePolygon = Polygon(basePolygon);
}

void Tile::initWallPolygons() {

    // TODO: Explain these polygons
    Cartesian lowerLeftPoint = m_basePolygon.getVertices().at(0);
    Cartesian upperLeftPoint = m_basePolygon.getVertices().at(1);
    Cartesian upperRightPoint = m_basePolygon.getVertices().at(2);
    Cartesian lowerRightPoint = m_basePolygon.getVertices().at(3);

    Meters halfWallWidth = Meters(P()->wallWidth()) / 2.0;

    if (isWall(NORTH)) {
        std::vector<Cartesian> northWall;
        northWall.push_back(Cartesian(upperLeftPoint.getX() + halfWallWidth, upperLeftPoint.getY() - halfWallWidth));
        northWall.push_back(Cartesian(upperLeftPoint.getX() + halfWallWidth, upperLeftPoint.getY()));
        northWall.push_back(Cartesian(upperRightPoint.getX() - halfWallWidth, upperRightPoint.getY()));
        northWall.push_back(Cartesian(upperRightPoint.getX() - halfWallWidth, upperRightPoint.getY() - halfWallWidth));
        m_wallPolygons.push_back(Polygon(northWall));
    }
    if (isWall(EAST)) {
        std::vector<Cartesian> eastWall;
        eastWall.push_back(Cartesian(lowerRightPoint.getX() - halfWallWidth, lowerRightPoint.getY() + halfWallWidth));
        eastWall.push_back(Cartesian(upperRightPoint.getX() - halfWallWidth, upperRightPoint.getY() - halfWallWidth));
        eastWall.push_back(Cartesian(upperRightPoint.getX(), upperRightPoint.getY() - halfWallWidth));
        eastWall.push_back(Cartesian(lowerRightPoint.getX(), lowerRightPoint.getY() + halfWallWidth));
        m_wallPolygons.push_back(Polygon(eastWall));
    }
    if (isWall(SOUTH)) {
        std::vector<Cartesian> southWall;
        southWall.push_back(Cartesian(lowerLeftPoint.getX() + halfWallWidth, lowerLeftPoint.getY()));
        southWall.push_back(Cartesian(lowerLeftPoint.getX() + halfWallWidth, lowerLeftPoint.getY() + halfWallWidth));
        southWall.push_back(Cartesian(lowerRightPoint.getX() - halfWallWidth, lowerRightPoint.getY() + halfWallWidth));
        southWall.push_back(Cartesian(lowerRightPoint.getX() - halfWallWidth, lowerRightPoint.getY()));
        m_wallPolygons.push_back(Polygon(southWall));
    }
    if (isWall(WEST)) {
        std::vector<Cartesian> westWall;
        westWall.push_back(Cartesian(lowerLeftPoint.getX(), lowerLeftPoint.getY() + halfWallWidth));
        westWall.push_back(Cartesian(upperLeftPoint.getX(), upperLeftPoint.getY() - halfWallWidth));
        westWall.push_back(Cartesian(upperLeftPoint.getX() + halfWallWidth, upperLeftPoint.getY() - halfWallWidth));
        westWall.push_back(Cartesian(lowerLeftPoint.getX() + halfWallWidth, lowerLeftPoint.getY() + halfWallWidth));
        m_wallPolygons.push_back(Polygon(westWall));
    }
}

void Tile::initCornerPolygons() {

    Cartesian lowerLeftPoint = m_basePolygon.getVertices().at(0);
    Cartesian upperLeftPoint = m_basePolygon.getVertices().at(1);
    Cartesian upperRightPoint = m_basePolygon.getVertices().at(2);
    Cartesian lowerRightPoint = m_basePolygon.getVertices().at(3);

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
