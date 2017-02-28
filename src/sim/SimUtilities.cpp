#include "SimUtilities.h"

#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QString>
#include <QThread>
#include <QTime>
#include <QtAlgorithms>

#include <random>

#include "Assert.h"
#include "Directory.h"
#include "Logging.h"
#include "Param.h"

namespace mms {

void SimUtilities::quit() {
    // TODO: MACK - make a better API for exiting from each of the threads
    exit(1);
}

double SimUtilities::getRandom() {
    
    // The '- 1' ensures that the random number is never 1.
    // This matches the python implementation where random is [0,1).
    // This is particularly useful if you want to index into array like so
    // array[std::floor(random * <number of elements>)] without having to check
    // the condition if this function returns 1.
    
    static std::mt19937 generator(P()->randomSeed());
    return std::abs(static_cast<double>(generator()) - 1) / static_cast<double>(generator.max());
}

void SimUtilities::sleep(const Duration& duration) {
    ASSERT_LE(0, duration.getMicroseconds());
    QThread::usleep(duration.getMicroseconds());
}

double SimUtilities::getHighResTimestamp() {
    return QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
}

QString SimUtilities::timestampToDatetimeString(const Duration& timestamp) {
    QDateTime timestampObject;
    timestampObject.setTime_t(timestamp.getSeconds());
    return timestampObject.toString("yyyy-MM-dd hh:mm:ss");
}

QString SimUtilities::formatDuration(const Duration& duration) {
    return QTime(0, 0, 0)
        .addMSecs(duration.getMilliseconds())
        .toString("mm:ss.zzz");
}

bool SimUtilities::isBool(const QString& str) {
    return str == "true" || str == "false";
}

bool SimUtilities::isInt(const QString& str) {
    bool ok;
    str.toInt(&ok);
    return ok;
}

bool SimUtilities::isDouble(const QString& str) {
    bool ok;
    str.toDouble(&ok);
    return ok;
}

bool SimUtilities::strToBool(const QString& str) {
    ASSERT_TR(isBool(str));
    return str == "true";
}

char SimUtilities::strToChar(const QString& str) {
    ASSERT_EQ(str.size(), 1);
    return str.at(0).toLatin1();
}

int SimUtilities::strToInt(const QString& str) {
    ASSERT_TR(isInt(str));
    return str.toInt();
}

double SimUtilities::strToDouble(const QString& str) {
    ASSERT_TR(isDouble(str));
    return str.toDouble();
}

QString SimUtilities::boolToStr(bool value) {
    return value ? "true" : "false";
}

QPair<QStringList, QStringList> SimUtilities::getFiles(const QString& dirPath) {
    QDir dir(dirPath);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QDirIterator iterator(dir, QDirIterator::Subdirectories);
    QStringList relativePaths;
    QStringList absolutePaths;
    while (iterator.hasNext()) {
        iterator.next();
        relativePaths << iterator.fileName();
        absolutePaths << iterator.filePath();
    }
    return {relativePaths, absolutePaths};
}

QStringList SimUtilities::getTopLevelDirs(const QString& dirPath) {
    QDir dir(dirPath);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name | QDir::QDir::IgnoreCase);
    QStringList topLevelDirs = dir.entryList();
    return topLevelDirs;
}

void SimUtilities::removeExcessArchivedRuns() {
    // Information about each run is stored in the run/ directory. As it turns
    // out, this information can pile up pretty quickly. We should remove the
    // oldest stuff so that the run/ directory doesn't get too full.
    QStringList runDirectories = getTopLevelDirs(Directory::get()->getRunDirectory());
    qSort(runDirectories);
    for (int i = 0; i < runDirectories.size() - P()->numberOfArchivedRuns(); i += 1) {
        QDir dir(Directory::get()->getRunDirectory() + runDirectories.at(i));
        bool success = dir.removeRecursively();
        if (!success) {
            qWarning() << "Unable to delete old run directory:" << dir.path();
        }
    }
}

QVector<TriangleGraphic> SimUtilities::polygonToTriangleGraphics(
        const Polygon& polygon,
        Color color,
        double alpha) {
    QVector<Triangle> triangles = polygon.getTriangles();
    QVector<TriangleGraphic> triangleGraphics;
    RGB colorValues = COLOR_TO_RGB.value(color);
    for (Triangle triangle : triangles) {
        triangleGraphics.push_back({
            {triangle.p1.getX().getMeters(), triangle.p1.getY().getMeters(), colorValues, alpha},
            {triangle.p2.getX().getMeters(), triangle.p2.getY().getMeters(), colorValues, alpha},
            {triangle.p3.getX().getMeters(), triangle.p3.getY().getMeters(), colorValues, alpha}
        });
    }
    return triangleGraphics;
}

} // namespace mms
