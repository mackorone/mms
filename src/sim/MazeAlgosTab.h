#pragma once

#include <QComboBox>
#include <QProcess>
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

    void edit();
    void import();

    void startBuild();
    void stopBuild();

    void startRun();
    void stopRun();

signals:

    void mazeGenerated(const QByteArray& bytes);

private:

    QComboBox* m_comboBox;
    QPushButton* m_editButton;
    QPushButton* m_importButton;

    QProcess* m_buildProcess;
    QPushButton* m_startBuildButton;
    QPushButton* m_stopBuildButton;
    TextDisplayWidget* m_buildDisplay;

    QProcess* m_runProcess;
    QPushButton* m_startRunButton;
    QPushButton* m_stopRunButton;
    TextDisplayWidget* m_runDisplay;

    QSpinBox* m_widthBox;
    QSpinBox* m_heightBox;

    RandomSeedWidget* m_seedWidget;

    void refresh(const QString& name = "");
    QVector<ConfigDialogField> getFields();

};

} // namespace mms
