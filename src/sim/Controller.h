#pragma once

#include <QObject>
#include <QProcess>

#include "DynamicMouseAlgorithmOptions.h"
#include "Lens.h"
#include "Model.h"
#include "MouseInterface.h"

namespace mms {

class Controller : public QObject {

    Q_OBJECT

public:

    Controller(Model* model, Lens* lens, const QString& mouseAlgorithm);

    // TODO: MACK
    InterfaceType getInterfaceType();

    DynamicMouseAlgorithmOptions getDynamicOptions();

    void init();
    void processMouseAlgoStderr();
    void processMouseAlgoStdout(); // TODO: MACK

signals:

    void algoStdout(const QString& line);

private:

    Model* m_model;
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

    // Whether or not the static options have been finalized
    bool m_staticOptionsFinalized;

};

} // namespace mms
