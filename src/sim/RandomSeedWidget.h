#pragma once

#include <QSpinBox>

namespace mms {

class RandomSeedWidget : public QWidget {

    Q_OBJECT

public:

    RandomSeedWidget(int max = 9999);
    int next();

private:

    int m_max;
    int getNext();

    QSpinBox* m_prevSeedBox;
    QSpinBox* m_nextSeedBox;

};

} // namespace mms
