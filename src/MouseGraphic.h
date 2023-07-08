#pragma once

#include <QVector>

#include "Mouse.h"
#include "TriangleGraphic.h"

namespace mms {

class MouseGraphic {
 public:
  MouseGraphic(const Mouse *mouse);
  QVector<TriangleGraphic> draw() const;

 private:
  const Mouse *m_mouse;
};

}  // namespace mms
