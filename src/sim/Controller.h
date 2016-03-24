#pragma once

#include "Model.h"
#include "View.h"

#include "../mouse/IMouseAlgorithm.h"
#include "MouseInterface.h"
#include "units/Seconds.h"

namespace sim {

class Controller {

public:
    Controller(Model* model, View* view);

    IMouseAlgorithm* getMouseAlgorithm();
    MouseInterface* getMouseInterface();

private:

    Model* m_model; // TODO: MACK - don't need this..
    View* m_view;

    IMouseAlgorithm* m_mouseAlgorithm;
    MouseInterface* m_mouseInterface;

    void validateMouseAlgorithm(const std::string& mouseAlgorithm);
    void validateMouseInterfaceType(
        const std::string& mouseAlgorithm, const std::string& interfaceType);
    void validateMouseInitialDirection(
        const std::string& mouseAlgorithm, const std::string& initialDirection);
    Direction getInitialDirection(const std::string& initialDirectionString);
    void validateMouseWheelSpeedFraction(
        const std::string& mouseAlgorithm, double wheelSpeedFraction);
    void validateTileTextRowsAndCols(
        const std::string& mouseAlgorithm,
        int tileTextNumberOfRows, int tileTextNumberOfCols);
    void initAndValidateMouse(
        const std::string& mouseAlgorithm, const std::string& mouseFile,
        InterfaceType interfaceType, Direction initialDirection, Mouse* mouse);

};

} // namespace sim
