#pragma once

#include "Model.h"
#include "View.h"
#include "ControllerManager.h"

namespace mms {

class Driver {

public:
    Driver() = delete;
    static int drive(int argc, char* argv[]);

private:
    static Model* m_model;
    static View* m_view;
    static ControllerManager* m_controllerManager;

};

} // namespace mms
