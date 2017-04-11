#pragma once

#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "ConfigDialogField.h"
#include "RandomSeedWidget.h"
#include "TextDisplayWidget.h"

namespace mms {

class MazeAlgosTab : public QWidget {

    Q_OBJECT

public:

    MazeAlgosTab();
    void import();
    void edit();
    void build();
    void run();

signals:

    void mazeGenerated(const QByteArray& bytes);

private:

    QComboBox* m_comboBox;
    QPushButton* m_editButton;
    QPushButton* m_buildButton;
    QPushButton* m_runButton;

    QSpinBox* m_widthBox;
    QSpinBox* m_heightBox;

    RandomSeedWidget* m_seedWidget;

    TextDisplayWidget* m_buildDisplay;
    TextDisplayWidget* m_runDisplay;

    void refresh(const QString& name = "");
    QVector<ConfigDialogField> getFields();

};

} // namespace mms
