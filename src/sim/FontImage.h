#pragma once

#include <QChar>
#include <QMap>
#include <QPair>

namespace mms {

class FontImage {

public:
    static void init();
    static FontImage* get();
	QMap<QChar, QPair<double, double>> positions();

private:
    FontImage();
    static FontImage* INSTANCE;
	QMap<QChar, QPair<double, double>> m_positions;

};

} // namespace mms
