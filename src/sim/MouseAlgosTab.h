#pragma once

#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "ConfigDialogField.h"
#include "RandomSeedWidget.h"
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
    QPushButton* m_buildButton;
    QPushButton* m_runStopButton;
    QPushButton* m_pauseButton;

    RandomSeedWidget* m_seedWidget;

    TextDisplayWidget* m_buildDisplay;
    TextDisplayWidget* m_runDisplay;

    void import();
    void edit();
    void build();
    void run();

    void refresh(const QString& name = "");
    QVector<ConfigDialogField> getFields();

};

} // namespace mms
