#pragma once

#include <QObject>
#include <QProcess>

#include "DynamicMouseAlgorithmOptions.h"
#include "Lens.h"
#include "Maze.h"
#include "Mouse.h"
#include "MouseInterface.h"
#include "World.h"

namespace mms {

class Controller : public QObject {

    Q_OBJECT

public:

    Controller(
        const World* world, 
        const Maze* maze,
        Mouse* mouse,
        Lens* lens,
        const QString& mouseAlgorithm);

    // TODO: MACK
    InterfaceType getInterfaceType();

    DynamicMouseAlgorithmOptions getDynamicOptions();

    void init();
    void processMouseAlgoStderr();
    void processMouseAlgoStdout(); // TODO: MACK

signals:

    void algoStdout(const QString& line);

private:

    const World* m_world;
    const Maze* m_maze;
    Mouse* m_mouse;
    Lens* m_lens;

    // TODO: MACK - defaults to DISCRETE
    InterfaceType m_interfaceType;
    bool m_interfaceTypeFinalized;

    QString m_mouseAlgorithm;

    QProcess* m_process;
    QStringList m_inputLines;

    QString processCommand(const QString& command);
    void startMouseAlgorithm(const QString& mouseAlgorithm);

    // The algorithm options
    DynamicMouseAlgorithmOptions m_dynamicOptions;

    // The interface by which we access the Mouse object
    MouseInterface* m_mouseInterface;

};

} // namespace mms
