#pragma once

#include <QObject>
#include <QProcess>

#include "DynamicMouseAlgorithmOptions.h"
#include "Lens.h"
#include "Model.h"
#include "MouseInterface.h"
#include "StaticMouseAlgorithmOptions.h"

namespace mms {

class Controller : public QObject {

    Q_OBJECT

public:
    Controller(Model* model, Lens* lens, const QString& mouseAlgorithm);

    StaticMouseAlgorithmOptions getStaticOptions();
    DynamicMouseAlgorithmOptions getDynamicOptions();

public slots:

    void init();
    void processMouseAlgoStderr();
    void processMouseAlgoStdout();

private:

    Model* m_model;
    Lens* m_lens;

    QString m_mouseAlgorithm;

    QProcess* m_process;
    QStringList m_inputLines;

    QString processCommand(const QString& command);
    void startMouseAlgorithm(const QString& mouseAlgorithm);

    // The algorithm options
    StaticMouseAlgorithmOptions m_staticOptions;
    DynamicMouseAlgorithmOptions m_dynamicOptions;

    // The interface by which we access the Mouse object
    MouseInterface* m_mouseInterface;

    // Whether or not the static options have been finalized
    bool m_staticOptionsFinalized;

    void validateMouseInterfaceType(
        const QString& mouseAlgorithm,
        const QString& interfaceType);

};

} // namespace mms
