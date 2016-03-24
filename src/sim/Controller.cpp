#include "Controller.h"

#include "ContainerUtilities.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"

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
    // TODO: MACK - validate tileTextArgs

    initAndValidateMouse(
        P()->mouseAlgorithm(),
        m_mouseAlgorithm->mouseFile(),
        STRING_TO_INTERFACE_TYPE.at(m_mouseAlgorithm->interfaceType()),
        getInitialDirection(m_mouseAlgorithm->initialDirection()),
        m_model->getMouse()
    );

    m_mouseInterface = new MouseInterface(
        m_model->getMaze(),
        m_model->getMouse(),
        m_view->getMazeGraphic(),
        m_view->getAllowableTileTextCharacters(),
        {
            m_mouseAlgorithm->allowOmniscience(),
            m_mouseAlgorithm->setTileTextWhenDistanceDeclared(),
            m_mouseAlgorithm->setTileBaseColorWhenDistanceDeclaredCorrectly(),
            m_mouseAlgorithm->tileTextNumberOfRows(),
            m_mouseAlgorithm->tileTextNumberOfCols(),
            m_mouseAlgorithm->declareWallOnRead(),
            m_mouseAlgorithm->declareBothWallHalves(),
            m_mouseAlgorithm->useTileEdgeMovements(),
            m_mouseAlgorithm->wheelSpeedFraction(),
            STRING_TO_INTERFACE_TYPE.at(m_mouseAlgorithm->interfaceType()),
        }
    );
}

IMouseAlgorithm* Controller::getMouseAlgorithm() {
    return m_mouseAlgorithm;
}

MouseInterface* Controller::getMouseInterface() {
    return m_mouseInterface;
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

} // namespace sim
