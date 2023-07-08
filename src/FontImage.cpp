#include "FontImage.h"

namespace mms {

QString FontImage::path() { return ":/resources/fonts/Unispace-Bold.png"; }

QString FontImage::characters() {
  // Must match the font image, else the wrong
  // characters will be displayed on the tiles
  return (
      " !\"#$%&'()*+,-./0123456789:;<=>?"
      "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
      "`abcdefghijklmnopqrstuvwxyz{|}~");
}

QMap<QChar, QPair<double, double>> FontImage::positions() {
  static QMap<QChar, QPair<double, double>> map;
  if (map.isEmpty()) {
    // Map from char to fractional position in the image (from 0.0 to 1.0)
    QString chars = characters();
    int size = chars.size();
    for (int i = 0; i < size; i += 1) {
      double start = static_cast<double>(i) / static_cast<double>(size);
      double end = static_cast<double>(i + 1) / static_cast<double>(size);
      map.insert(chars.at(i), {start, end});
    }
  }
  return map;
}

}  // namespace mms
