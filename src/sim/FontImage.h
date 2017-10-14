#pragma once

#include <QChar>
#include <QMap>
#include <QPair>

namespace mms {

class FontImage {

public:
    static void init(const QString& imageFile);
    static FontImage* get();
    QString imageFilePath();
    QMap<QChar, QPair<double, double>> positions();

private:
    FontImage(const QString& imageFile);
    static FontImage* INSTANCE;
    QString m_imageFilePath;
    QMap<QChar, QPair<double, double>> m_positions;

};

} // namespace mms
