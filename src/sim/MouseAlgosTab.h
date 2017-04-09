#pragma once

#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "ConfigDialogField.h"
#include "TextDisplayWidget.h"

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
        TextDisplayWidget* display);

private:

    QComboBox* m_comboBox;

    QPushButton* m_editButton;

    QSpinBox* m_seedBox;
    QCheckBox* m_seedAutoUpdate;

    QPushButton* m_buildButton;
    QPushButton* m_runButton;

    TextDisplayWidget* m_buildDisplay;
    TextDisplayWidget* m_runDisplay;

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
