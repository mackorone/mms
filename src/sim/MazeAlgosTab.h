#pragma once

#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QWidget>

#include "ConfigDialogField.h"
#include "TextDisplay.h"

namespace mms {

class MazeAlgosTab : public QWidget {

    Q_OBJECT

public:

    MazeAlgosTab();

private:

    QComboBox* m_comboBox;

    QPushButton* m_buildButton;
    TextDisplay* m_buildOutput;
    QCheckBox* m_buildAutoClear;

    QPushButton* m_runButton;
    TextDisplay* m_runOutput;
    QCheckBox* m_runAutoClear;

    void import();
    void edit();
    void build();
    void run();

    void refresh(const QString& name = "");
    QVector<ConfigDialogField> getFields();

};

} // namespace mms
