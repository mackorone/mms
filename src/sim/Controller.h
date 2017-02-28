#pragma once

#include <QObject>
#include <QProcess>

#include "DynamicMouseAlgorithmOptions.h"
#include "Maze.h"
#include "MazeViewMutable.h"
#include "Mouse.h"
#include "MouseInterface.h"

namespace mms {

class Controller : public QObject {

    Q_OBJECT

public:

    Controller(const Maze* maze, Mouse* mouse, MazeViewMutable* view);
    void init();
    void start(const QString& algoName);

    InterfaceType getInterfaceType(bool canFinalize);
    DynamicMouseAlgorithmOptions getDynamicOptions() const;

signals:

    void algoStdout(const QString& line);

private:

    const Maze* m_maze;
    Mouse* m_mouse;
    MazeViewMutable* m_view;

    InterfaceType m_interfaceType;
    bool m_interfaceTypeFinalized;

    // The interface by which we access the Mouse object
    MouseInterface* m_mouseInterface;

    // The runtime algorithm options
    DynamicMouseAlgorithmOptions m_dynamicOptions;

    // The subprocess on which the mouse algo executes
    QProcess* m_process;

    // Buffers containing incomplete algo stdout/stderr lines
    QStringList m_stdoutBuffer;
    QStringList m_stderrBuffer;

    // Given some text (and a buffer containing past input), return
    // all complete lines and append remaining text to the buffer
    QStringList getLines(const QString& text, QStringList* buffer);

    // Process a single command, as sent by a mouse algorithm
    QString processCommand(const QString& command);

};

} // namespace mms
