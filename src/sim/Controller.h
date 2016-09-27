#pragma once

#include <QObject>
#include <QProcess>
#include <QString>

#include "Direction.h"
#include "DynamicMouseAlgorithmOptions.h"
#include "Model.h"
#include "MouseInterface.h"
#include "StaticMouseAlgorithmOptions.h"
#include "View.h"

namespace mms {

class Controller : public QObject {

    Q_OBJECT

public:

    Controller(Model* model, View* view);

    MouseInterface* getMouseInterface();

    StaticMouseAlgorithmOptions getStaticOptions();
    DynamicMouseAlgorithmOptions getDynamicOptions();

private:

    // The mouse algorithm child process
    QProcess* m_process;

    // The lines of input from the mouse algorithm
    QStringList m_inputLines;

    // The interface by which we access the Mouse object
    MouseInterface* m_mouseInterface;

    // The algorithm options
    StaticMouseAlgorithmOptions m_staticOptions;
    DynamicMouseAlgorithmOptions m_dynamicOptions;

    // Whether or not the static options have been finalized
    bool m_staticOptionsFinalized;

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

    Direction getInitialDirection(
        const QString& initialDirection,
        Model* model);

    QString processCommand(const QString& command);

private slots:

    void readAndProcessCommands();

// TODO: MACK - refactor these somewhere else
private:

    void startMouseAlgorithm(const QString& mouseAlgorithm);
    static QStringList getMouseAlgos();
    static QPair<QStringList, QStringList> getFiles(const QString& dirPath);

};

} // namespace mms
