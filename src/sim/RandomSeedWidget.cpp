#include "RandomSeedWidget.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>

#include "SimUtilities.h"

namespace mms {

RandomSeedWidget::RandomSeedWidget(int max) :
        m_max(max),
        m_prevSeedBox(new QSpinBox()),
        m_nextSeedBox(new QSpinBox()) {

    m_prevSeedBox->setReadOnly(true);
    m_prevSeedBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_prevSeedBox->setSpecialValueText("N/A");
    m_prevSeedBox->setRange(0, m_max);
    m_nextSeedBox->setRange(0, m_max);
    m_nextSeedBox->setValue(getNext());

    QHBoxLayout* layout = new QHBoxLayout();
    QGroupBox* groupBox = new QGroupBox("Random Seed");
    QHBoxLayout* groupBoxLayout = new QHBoxLayout();
    groupBoxLayout->addWidget(new QLabel("Previous"));
    groupBoxLayout->addWidget(m_prevSeedBox);
    groupBoxLayout->addWidget(new QLabel("Next"));
    groupBoxLayout->addWidget(m_nextSeedBox);
    groupBox->setLayout(groupBoxLayout);
    layout->addWidget(groupBox);
    setLayout(layout);
}

int RandomSeedWidget::next() {
    int seed = m_nextSeedBox->value();
    m_prevSeedBox->setValue(seed);
    m_prevSeedBox->setSpecialValueText("");
    m_nextSeedBox->setValue(getNext());
    return seed;
}

int RandomSeedWidget::getNext() {
    return SimUtilities::randomNonNegativeInt(m_max + 1);
}

} // namespace mms
