#pragma once

#include <QObject>

namespace mms {

// TODO: MACK
class Controller;

class Worker : public QObject {

    Q_OBJECT

public:
    // TODO: MACK
    Worker(Controller* controller);

signals:
    void commandProcessed(const QString& result);

public slots:
    void processCommand(const QString& command);

private:
    Controller* m_controller;
    QString helper(const QString& command);

};

} // namespace mms
