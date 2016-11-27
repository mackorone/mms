#pragma once

#include <QObject>
#include <QProcess>

#include "MouseInterface.h"

namespace mms {

class Controller;
class Model;
class View;

class Worker : public QObject {

    Q_OBJECT

public:
    // TODO: MACK
    Worker(Model* model, View* view, Controller* controller);

public slots:
    void init();
    void processMouseAlgoStderr();
    void processMouseAlgoStdout();

private:
    Model* m_model;
    View* m_view;
    Controller* m_controller;

    QProcess* m_process;
    QStringList m_inputLines;

    QString helper(const QString& command);
    void startMouseAlgorithm(const QString& mouseAlgorithm);

    // TODO: MACK

    // The interface by which we access the Mouse object
    MouseInterface* m_mouseInterface;

    // Whether or not the static options have been finalized
    bool m_staticOptionsFinalized;

    Direction getInitialDirection(
        const QString& initialDirection,
        Model* model);

    void validateMouseInterfaceType(
        const QString& mouseAlgorithm,
        const QString& interfaceType);

    void validateMouseInitialDirection(
        const QString& mouseAlgorithm,
        const QString& initialDirection);

    void validateTileTextRowsAndCols(
        const QString& mouseAlgorithm,
        int tileTextNumberOfRows,
        int tileTextNumberOfCols);

    void validateMouseWheelSpeedFraction(
        const QString& mouseAlgorithm,
        double wheelSpeedFraction);

    void initAndValidateMouse(
        const QString& mouseAlgorithm,
        const QString& mouseFile,
        const QString& interfaceType,
        const QString& initialDirection,
        Model* model);

};

} // namespace mms
