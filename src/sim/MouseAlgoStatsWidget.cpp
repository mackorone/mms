#include "MouseAlgoStatsWidget.h"

#include <QGridLayout>
#include <QLabel>

namespace mms {

void MouseAlgoStatsWidget::init(QStringList keys) {
    QGridLayout* layout = new QGridLayout();
    setLayout(layout);
    for (int i = 0; i < keys.size(); i += 1) {
        QString label = keys.at(i);
        QLabel* labelHolder = new QLabel(label + ":");
        QLabel* valueHolder = new QLabel();
        valueHolder->setAlignment(Qt::AlignCenter);
        valueHolder->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        valueHolder->setMinimumWidth(80);
        layout->addWidget(labelHolder, i, 0);
        layout->addWidget(valueHolder, i, 1);
    }
}

} 
