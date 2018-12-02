#include "SimUtilities.h"

#include <QDateTime>

#include "Assert.h"

namespace mms {

double SimUtilities::getHighResTimestamp() {
    return QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
}

bool SimUtilities::isBool(const QString& str) {
    return str == "true" || str == "false";
}

bool SimUtilities::isInt(const QString& str) {
    bool ok;
    str.toInt(&ok);
    return ok;
}

bool SimUtilities::strToBool(const QString& str) {
    ASSERT_TR(isBool(str));
    return str == "true";
}

QChar SimUtilities::strToChar(const QString& str) {
    ASSERT_EQ(str.size(), 1);
    return str.at(0);
}

int SimUtilities::strToInt(const QString& str) {
    ASSERT_TR(isInt(str));
    return str.toInt();
}

QString SimUtilities::boolToStr(bool value) {
    return value ? "true" : "false";
}

QVector<TriangleGraphic> SimUtilities::polygonToTriangleGraphics(
        const Polygon& polygon,
        Color color,
        unsigned char alpha) {
    QVector<Triangle> triangles = polygon.getTriangles();
    QVector<TriangleGraphic> triangleGraphics;
    RGB colorValues = COLOR_TO_RGB().value(color);
    for (Triangle triangle : triangles) {
        TriangleGraphic graphic;
        graphic.p1 = {
            triangle.p1.getX().getMeters(),
            triangle.p1.getY().getMeters(),
            colorValues,
            alpha,
        };
        graphic.p2 = {
            triangle.p2.getX().getMeters(),
            triangle.p2.getY().getMeters(),
            colorValues,
            alpha,
        };
        graphic.p3 = {
            triangle.p3.getX().getMeters(),
            triangle.p3.getY().getMeters(),
            colorValues,
            alpha,
        };
        triangleGraphics.append(graphic);
    }
    return triangleGraphics;
}

} 
