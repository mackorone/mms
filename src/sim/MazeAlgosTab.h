#pragma once

#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "ConfigDialogField.h"
#include "TextDisplayWidget.h"

namespace mms {

class MazeAlgosTab : public QWidget {

    Q_OBJECT

public:

    MazeAlgosTab();

signals:

    void mazeGenerated(const QByteArray& bytes);

private:

    static const int MAX_SEED_VALUE = 9999;

    QComboBox* m_comboBox;
    QPushButton* m_editButton;

    QSpinBox* m_widthBox;
    QSpinBox* m_heightBox;

    QSpinBox* m_prevSeedBox;
    QSpinBox* m_nextSeedBox;

    QPushButton* m_buildButton;
    QPushButton* m_runButton;

    TextDisplayWidget* m_buildDisplay;
    TextDisplayWidget* m_runDisplay;

    void import();
    void edit();
    void build();
    void run();

    int getNextSeed();
    void refresh(const QString& name = "");
    QVector<ConfigDialogField> getFields();

};

} // namespace mms
