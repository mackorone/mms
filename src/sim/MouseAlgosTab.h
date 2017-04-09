#pragma once

#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "ConfigDialogField.h"
#include "TextDisplay.h"

namespace mms {

class MouseAlgosTab : public QWidget {

    Q_OBJECT

public:

    MouseAlgosTab();
    void mouseAlgoStopped();

signals:

    void stopRequested();
    void pauseButtonPressed(bool pause);
    void simSpeedChanged(double factor);
    void mouseAlgoSelected(
        const QString& name,
        const QString& runCommand,
        const QString& directory,
        const QString& mouseFilePath,
        int seed,
        TextDisplay* display);

private:

    QComboBox* m_comboBox;

    QPushButton* m_editButton;

    QSpinBox* m_seedBox;
    QCheckBox* m_seedAutoUpdate;

    QPushButton* m_buildButton;
    TextDisplay* m_buildOutput;
    QCheckBox* m_buildAutoClear;

    QPushButton* m_runButton;
    TextDisplay* m_runOutput;
    QCheckBox* m_runAutoClear;

    QPushButton* m_stopButton;
    QPushButton* m_pauseButton;

    void import();
    void edit();
    void build();
    void run();

    void refresh(const QString& name = "");
    QVector<ConfigDialogField> getFields();

};

} // namespace mms
