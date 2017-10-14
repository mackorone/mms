#pragma once

#include <QCheckBox>
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

    QCheckBox* m_lockSeed;
    QSpinBox* m_nextSeedBox;
    QSpinBox* m_prevSeedBox;

};

} // namespace mms
