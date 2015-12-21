#pragma once

#include "Model.h"
#include "View.h"

#include "../mouse/IMouseAlgorithm.h"
#include "MouseInterface.h"
#include "World.h"

namespace sim {

class Controller {

public:
    Controller(Model* model, View* view);

    IMouseAlgorithm* getMouseAlgorithm();
    MouseInterface* getMouseInterface();
    World* getWorld();

    void keyPress(unsigned char key, int x, int y);
    void specialKeyPress(int key, int x, int y);
    void specialKeyRelease(int key, int x, int y);

private:

    // TODO: MACK
    Model* m_model;
    View* m_view;

    // TODO: MACK - make into pair
    IMouseAlgorithm* m_mouseAlgorithm;
    MouseInterface* m_mouseInterface;
    World* m_world; // TODO: MACK Rename this...

    // Initialize all of the simulation objects
    void validateMouseAlgorithm(const std::string& mouseAlgorithm);
    void validateMouseInterfaceType(
        const std::string& mouseAlgorithm, const std::string& interfaceType);
    void validateMouseInitialDirection(
        const std::string& mouseAlgorithm, const std::string& initialDirection);
    Direction getInitialDirection(const std::string& initialDirectionString);
    void validateMouseWheelSpeedFraction(
        const std::string& mouseAlgorithm, double wheelSpeedFraction);
    void initAndValidateMouse(
        const std::string& mouseAlgorithm, const std::string& mouseFile,
        InterfaceType interfaceType, Direction initialDirection, Mouse* mouse);

};

} // namespace sim
