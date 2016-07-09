#pragma once

#include "Model.h"
#include "View.h"
#include "Controller.h"

namespace sim {

class Driver {

public:
    Driver() = delete;
    static void drive(int argc, char* argv[]);

private:
    static Model* m_model;
    static View* m_view;
    static Controller* m_controller;

};

} // namespace sim
