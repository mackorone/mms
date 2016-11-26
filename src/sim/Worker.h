#pragma once

#include <QObject>
#include <QProcess>

namespace mms {

// TODO: MACK
class Controller;

class Worker : public QObject {

    Q_OBJECT

public:
    // TODO: MACK
    Worker(Controller* controller);

public slots:
    void init();
    void processMouseAlgoStderr();
    void processMouseAlgoStdout();

private:
    Controller* m_controller;
    QProcess* m_process;
    QStringList m_inputLines;

    QString helper(const QString& command);
    void startMouseAlgorithm(const QString& mouseAlgorithm);

};

} // namespace mms
