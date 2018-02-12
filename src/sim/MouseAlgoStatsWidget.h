#pragma once

#include <QStringList>
#include <QWidget>

namespace mms {

class MouseAlgoStatsWidget : public QWidget {

    Q_OBJECT

public:

    void init(QStringList keys);

};

} // namespace mms
