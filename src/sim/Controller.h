#pragma once

#include "Model.h"
#include "View.h"

#include "../mouse/IMouseAlgorithm.h"
#include "MouseInterface.h"
#include "StaticMouseAlgorithmOptions.h"

namespace sim {

class Controller {

public:
    Controller(Model* model, View* view);

    StaticMouseAlgorithmOptions getOptions();
    IMouseAlgorithm* getMouseAlgorithm();
    MouseInterface* getMouseInterface();

private:

    StaticMouseAlgorithmOptions m_options;
    IMouseAlgorithm* m_mouseAlgorithm;
    MouseInterface* m_mouseInterface;

    void validateMouseAlgorithm(
        const std::string& mouseAlgorithm);

    void validateMouseInterfaceType(
        const std::string& mouseAlgorithm,
        const std::string& interfaceType);

    void validateMouseInitialDirection(
        const std::string& mouseAlgorithm,
        const std::string& initialDirection);

    void validateTileTextRowsAndCols(
        const std::string& mouseAlgorithm,
        int tileTextNumberOfRows,
        int tileTextNumberOfCols);

    void validateMouseWheelSpeedFraction(
        const std::string& mouseAlgorithm,
        double wheelSpeedFraction);

    void initAndValidateMouse(
        const std::string& mouseAlgorithm,
        const std::string& mouseFile,
        const std::string& interfaceType,
        const std::string& initialDirection,
        Model* model);

    Direction getInitialDirection(
        const std::string& initialDirection,
        Model* model);

};

} // namespace sim
