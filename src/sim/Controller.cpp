#include "Controller.h"

#include "ContainerUtilities.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"

#include "../mouse/MouseAlgorithms.h"
#include "MouseChecker.h"
#include "MouseInterfaceOptions.h"

namespace sim {

// String used to specify that the mouse should start facing the opening
static const std::string& OPENING_DIRECTION_STRING = "OPENING";

// String used to specify that the mouse should start facing the wall
static const std::string& WALL_DIRECTION_STRING = "WALL";

Controller::Controller(Model* model, View* view) : m_model(model), m_view(view) {

    validateMouseAlgorithm(P()->mouseAlgorithm());
    m_mouseAlgorithm = MouseAlgorithms::getMouseAlgorithm(P()->mouseAlgorithm());
    validateMouseInterfaceType(P()->mouseAlgorithm(), m_mouseAlgorithm->interfaceType());
    validateMouseInitialDirection(P()->mouseAlgorithm(), m_mouseAlgorithm->initialDirection());
    validateMouseWheelSpeedFraction(P()->mouseAlgorithm(), m_mouseAlgorithm->wheelSpeedFraction());

    initAndValidateMouse(
        P()->mouseAlgorithm(),
        m_mouseAlgorithm->mouseFile(),
        STRING_TO_INTERFACE_TYPE.at(m_mouseAlgorithm->interfaceType()),
        getInitialDirection(m_mouseAlgorithm->initialDirection()),
        m_model->getMouse()
    );

    // TODO: MACK - validate here (non-negative)
    // We need to do this before we initialize the mouse interface
    m_view->initTileGraphicText(
        std::make_pair(
            m_mouseAlgorithm->tileTextNumberOfRows(),
            m_mouseAlgorithm->tileTextNumberOfCols()));

    // This may seem sloppy, but it's best to assign the values directly by
    // name so that we don't accidentally mix them up (which is easy to do)
    MouseInterfaceOptions options;
    options.allowOmniscience = m_mouseAlgorithm->allowOmniscience();
    options.declareWallOnRead = m_mouseAlgorithm->declareWallOnRead();
    options.declareBothWallHalves = m_mouseAlgorithm->declareBothWallHalves();
    options.tileTextNumberOfRows = m_mouseAlgorithm->tileTextNumberOfRows();
    options.tileTextNumberOfCols = m_mouseAlgorithm->tileTextNumberOfCols();
    options.setTileTextWhenDistanceDeclared =
        m_mouseAlgorithm->setTileTextWhenDistanceDeclared();
    options.setTileBaseColorWhenDistanceDeclaredCorrectly =
        m_mouseAlgorithm->setTileBaseColorWhenDistanceDeclaredCorrectly();
    options.useTileEdgeMovements = m_mouseAlgorithm->useTileEdgeMovements();
    options.wheelSpeedFraction = m_mouseAlgorithm->wheelSpeedFraction();
    options.interfaceType =
        STRING_TO_INTERFACE_TYPE.at(m_mouseAlgorithm->interfaceType());
    
    // Initialize the mouse interface
    m_mouseInterface = new MouseInterface(
        m_model->getMaze(),
        m_model->getMouse(),
        m_view->getMazeGraphic(),
        m_view->getAllowableTileTextCharacters(),
        options
    );

    // Separate out the World options here
    m_world = new World(
        m_model->getMaze(), 
        m_model->getMouse(), m_view->getMazeGraphic(),
        {
            m_mouseAlgorithm->automaticallyClearFog(),
            STRING_TO_INTERFACE_TYPE.at(m_mouseAlgorithm->interfaceType())
        }
    );

    // Tell the view about the world, so we can display more info in the header
    m_view->passWorldPointerToHeader(m_world);
}

IMouseAlgorithm* Controller::getMouseAlgorithm() {
    return m_mouseAlgorithm;
}

MouseInterface* Controller::getMouseInterface() {
    return m_mouseInterface;
}

World* Controller::getWorld() {
    return m_world;
}

void Controller::validateMouseAlgorithm(const std::string& mouseAlgorithm) {
    if (!MouseAlgorithms::isMouseAlgorithm(mouseAlgorithm)) {
        L()->error("\"%v\" is not a valid mouse algorithm.", mouseAlgorithm);
        SimUtilities::quit();
    }
}

void Controller::validateMouseInterfaceType(
        const std::string& mouseAlgorithm, const std::string& interfaceType) {
    if (!ContainerUtilities::mapContains(STRING_TO_INTERFACE_TYPE, interfaceType)) {
        L()->error(
            "\"%v\" is not a valid interface type. You must declare the "
            "interface type of the mouse algorithm \"%v\" to be either \"%v\" "
            "or \"%v\".",
            interfaceType,
            mouseAlgorithm,
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE),
            INTERFACE_TYPE_TO_STRING.at(InterfaceType::CONTINUOUS));
        SimUtilities::quit();
    }
}

void Controller::validateMouseInitialDirection(
        const std::string& mouseAlgorithm, const std::string& initialDirection) {
    if (!(ContainerUtilities::mapContains(STRING_TO_DIRECTION, initialDirection)
            || initialDirection == OPENING_DIRECTION_STRING
            || initialDirection == WALL_DIRECTION_STRING)) {
        L()->error(
            "\"%v\" is not a valid initial direction. You must declare the"
            " initial direction of the mouse algorithm \"%v\" to be one of"
            " \"%v\", \"%v\", \"%v\", \"%v\", \"%v\", or \"%v\".",
            initialDirection,
            mouseAlgorithm,
            DIRECTION_TO_STRING.at(Direction::NORTH),
            DIRECTION_TO_STRING.at(Direction::EAST),
            DIRECTION_TO_STRING.at(Direction::SOUTH),
            DIRECTION_TO_STRING.at(Direction::WEST),
            OPENING_DIRECTION_STRING,
            WALL_DIRECTION_STRING);
        SimUtilities::quit();
    }
}

Direction Controller::getInitialDirection(const std::string& initialDirectionString) {
    if (initialDirectionString == OPENING_DIRECTION_STRING) {
        return (m_model->getMaze()->getTile(0, 0)->isWall(Direction::EAST) ? Direction::NORTH : Direction::EAST);
    }
    if (initialDirectionString == WALL_DIRECTION_STRING) {
        return (m_model->getMaze()->getTile(0, 0)->isWall(Direction::NORTH) ? Direction::NORTH : Direction::EAST);
    }
    return STRING_TO_DIRECTION.at(initialDirectionString);
}

void Controller::validateMouseWheelSpeedFraction(
    const std::string& mouseAlgorithm, double wheelSpeedFraction) {
    if (!(0.0 <= wheelSpeedFraction && wheelSpeedFraction <= 1.0)) {
        L()->error(
            "\"%v\" is not a valid wheel speed fraction. The wheel speed"
            " fraction of the mouse algorithm \"%v\" has to be in [0.0, 1.0].",
            wheelSpeedFraction, 
            mouseAlgorithm);
        SimUtilities::quit();
    }
}

void Controller::initAndValidateMouse(
        const std::string& mouseAlgorithm, const std::string& mouseFile,
        InterfaceType interfaceType, Direction initialDirection, Mouse* mouse) {

    // Initialize the mouse with the file provided
    bool success = mouse->initialize(mouseFile, initialDirection);
    if (!success) {
        L()->error(
            "Unable to successfully initialize the mouse in the algorithm "
            "\"%v\" from \"%v\".",
            mouseAlgorithm,
            mouseFile);
        SimUtilities::quit();
    }

    // Validate the mouse
    if (interfaceType == InterfaceType::DISCRETE) {
        if (!MouseChecker::isDiscreteInterfaceCompatible(*mouse)) {
            L()->error("The mouse file \"%v\" is not discrete interface compatible.", mouseFile);
            SimUtilities::quit();
        }
    }
    else { // InterfaceType::CONTINUOUS
        if (!MouseChecker::isContinuousInterfaceCompatible(*mouse)) {
            L()->error("The mouse file \"%v\" is not continuous interface compatible.", mouseFile);
            SimUtilities::quit();
        }
    }
}

void Controller::keyPress(unsigned char key, int x, int y) {

    // NOTE: If you're adding or removing anything from this function, make
    // sure to update wiki/Keys.md

    if (key == 'p') {
        // Toggle pause (only in discrete mode)
        if (STRING_TO_INTERFACE_TYPE.at(getMouseAlgorithm()->interfaceType()) == InterfaceType::DISCRETE) {
            S()->setPaused(!S()->paused());
        }
        else {
            L()->warn(
                "Pausing the simulator is only allowed in %v mode.",
                INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE));
        }
    }
    else if (key == 'f') {
        // Faster (only in discrete mode)
        if (STRING_TO_INTERFACE_TYPE.at(getMouseAlgorithm()->interfaceType()) == InterfaceType::DISCRETE) {
            S()->setSimSpeed(S()->simSpeed() * 1.5);
        }
        else {
            L()->warn(
                "Increasing the simulator speed is only allowed in %v mode.",
                INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE));
        }
    }
    else if (key == 's') {
        // Slower (only in discrete mode)
        if (STRING_TO_INTERFACE_TYPE.at(getMouseAlgorithm()->interfaceType()) == InterfaceType::DISCRETE) {
            S()->setSimSpeed(S()->simSpeed() / 1.5);
        }
        else {
            L()->warn(
                "Decreasing the simulator speed is only allowed in %v mode.",
                INTERFACE_TYPE_TO_STRING.at(InterfaceType::DISCRETE));
        }
    }
    else if (key == 'l') {
        // Cycle through the available layouts
        S()->setLayoutType(LAYOUT_TYPE_CYCLE.at(S()->layoutType()));
    }
    else if (key == 'r') {
        // Toggle rotate zoomed map
        S()->setRotateZoomedMap(!S()->rotateZoomedMap());
    }
    else if (key == 'i') {
        // Zoom in
        S()->setZoomedMapScale(S()->zoomedMapScale() * 1.5);
    }
    else if (key == 'o') {
        // Zoom out
        S()->setZoomedMapScale(S()->zoomedMapScale() / 1.5);
    }
    else if (key == 't') {
        // Toggle wall truth visibility
        S()->setWallTruthVisible(!S()->wallTruthVisible());
        m_view->getMazeGraphic()->updateWalls();
    }
    else if (key == 'c') {
        // Toggle tile colors
        S()->setTileColorsVisible(!S()->tileColorsVisible());
        m_view->getMazeGraphic()->updateColor();
    }
    else if (key == 'g') {
        // Toggle tile fog
        S()->setTileFogVisible(!S()->tileFogVisible());
        m_view->getMazeGraphic()->updateFog();
    }
    else if (key == 'x') {
        // Toggle tile text
        S()->setTileTextVisible(!S()->tileTextVisible());
        m_view->getMazeGraphic()->updateText();
    }
    else if (key == 'd') {
        // Toggle tile distance visibility
        S()->setTileDistanceVisible(!S()->tileDistanceVisible());
        m_view->getMazeGraphic()->updateText();
    }
    else if (key == 'w') {
        // Toggle wireframe mode
        S()->setWireframeMode(!S()->wireframeMode());
        glPolygonMode(GL_FRONT_AND_BACK, S()->wireframeMode() ? GL_LINE : GL_FILL);
    }
    else if (key == 'q') {
        // Quit
        SimUtilities::quit();
    }
    else if (std::string("0123456789").find(key) != std::string::npos) {
        // Press an input button
        int inputButton = std::string("0123456789").find(key);
        if (!S()->inputButtonWasPressed(inputButton)) {
            S()->setInputButtonWasPressed(inputButton, true);
            L()->info("Input button %v was pressed.", inputButton);
        }
        else {
            L()->warn(
                "Input button %v has not yet been acknowledged as pressed; pressing it has no effect.",
                inputButton);
        }
    }
}

void Controller::specialKeyPress(int key, int x, int y) {
    if (!ContainerUtilities::mapContains(INT_TO_KEY, key)) {
        return;
    }
    if (ContainerUtilities::vectorContains(ARROW_KEYS, INT_TO_KEY.at(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.at(key), true);
    }
}

void Controller::specialKeyRelease(int key, int x, int y) {
    if (!ContainerUtilities::mapContains(INT_TO_KEY, key)) {
        return;
    }
    if (ContainerUtilities::vectorContains(ARROW_KEYS, INT_TO_KEY.at(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.at(key), false);
    }
}


} // namespace sim
