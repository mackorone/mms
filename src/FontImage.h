#pragma once

#include <QChar>
#include <QMap>
#include <QPair>

namespace mms {

class FontImage {
 public:
  FontImage() = delete;
  static QString path();
  static QString characters();
  static QMap<QChar, QPair<double, double>> positions();
};

}  // namespace mms
