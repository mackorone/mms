#pragma once

#include <QObject>
#include <QThread>
#include <QVector>

#include "Model.h"
#include "Map.h"
#include "Controller.h"

namespace mms {

class ControllerManager : public QObject {

    Q_OBJECT

public:

    ControllerManager(Model* model, Map* map);
    void spawnMouseAlgo(const QString& mouseAlgorithm);

private:

    // The model and map, which we pass to each controller
    Model* m_model;
    Map* m_map;

    // A list of controllers and the threads they're running on
    QVector<QPair<Controller*, QThread*>> m_controllers;

};

} // namespace mms
