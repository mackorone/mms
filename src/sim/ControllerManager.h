#pragma once

#include <QObject>
#include <QThread>
#include <QList>

#include "Model.h"
#include "View.h"
#include "Controller.h"

namespace mms {

class ControllerManager : public QObject {

    Q_OBJECT

public:

    ControllerManager(Model* model, View* view);
    void spawnMouseAlgo(const QString& mouseAlgorithm);

private:

    // The model and view, which we pass to each controller
    Model* m_model;
    View* m_view;

    // A list of controllers and the threads they're running on
    QList<QPair<Controller*, QThread*>> m_controllers;

};

} // namespace mms
