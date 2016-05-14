#include "Controller.h"

#include "ContainerUtilities.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"

#include "../mouse/MouseAlgorithms.h"
#include "MouseChecker.h"

namespace sim {

static const std::string& OPENING_DIRECTION_STRING = "OPENING";
static const std::string& WALL_DIRECTION_STRING = "WALL";

Controller::Controller(Model* model, View* view) {

    // Validate and initialize the mouse algorithm
    validateMouseAlgorithm(P()->mouseAlgorithm());
    m_mouseAlgorithm = MouseAlgorithms::getMouseAlgorithm(P()->mouseAlgorithm());

    // Read the static mouse algo options - only do this once
    m_options.mouseFile = m_mouseAlgorithm->mouseFile();
    m_options.interfaceType = m_mouseAlgorithm->interfaceType();
    m_options.initialDirection = m_mouseAlgorithm->initialDirection();
    m_options.tileTextNumberOfRows = m_mouseAlgorithm->tileTextNumberOfRows();
    m_options.tileTextNumberOfCols = m_mouseAlgorithm->tileTextNumberOfCols();
    m_options.wheelSpeedFraction = m_mouseAlgorithm->wheelSpeedFraction();

    // Validate all of the static options except for mouseFile,
    // which is validated in the mouse init method
    validateMouseInterfaceType(
        P()->mouseAlgorithm(),
        m_options.interfaceType
    );
    validateMouseInitialDirection(
        P()->mouseAlgorithm(),
        m_options.initialDirection
    );
    validateTileTextRowsAndCols(
        P()->mouseAlgorithm(),
        m_options.tileTextNumberOfRows,
        m_options.tileTextNumberOfCols
    );
    validateMouseWheelSpeedFraction(
        P()->mouseAlgorithm(),
        m_options.wheelSpeedFraction
    );

    initAndValidateMouse(
        P()->mouseAlgorithm(),
        m_options.mouseFile,
        m_options.interfaceType,
        m_options.initialDirection,
        model
    );

    m_mouseInterface = new MouseInterface(
        model->getMaze(),
        model->getMouse(),
        view->getMazeGraphic(),
        m_mouseAlgorithm,
        view->getAllowableTileTextCharacters(),
        m_options
    );
}

StaticMouseAlgorithmOptions Controller::getOptions() {
    // The Controller object is the source of truth for the static options
    return m_options;
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
    if (!(
        ContainerUtilities::mapContains(STRING_TO_DIRECTION, initialDirection)
        || initialDirection == OPENING_DIRECTION_STRING
        || initialDirection == WALL_DIRECTION_STRING
    )) {
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

void Controller::validateTileTextRowsAndCols(
    const std::string& mouseAlgorithm,
    int tileTextNumberOfRows, int tileTextNumberOfCols) {
    if (tileTextNumberOfRows < 0 || tileTextNumberOfCols < 0) {
        L()->error(
            "Both tileTextNumberOfRows() and tileTextNumberOfCols() must return"
            " non-negative integers. Since they return \"%v\" and \"%v\","
            " respectively, the tile text dimensions of the mouse algorithm"
            " \"%v\" are invalid.",
            tileTextNumberOfRows, 
            tileTextNumberOfCols,
            mouseAlgorithm);
        SimUtilities::quit();
    }
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
        const std::string& mouseAlgorithm,
        const std::string& mouseFile,
        const std::string& interfaceType,
        const std::string& initialDirection,
        Model* model) {

    // Initialize the mouse with the file provided
    Direction direction = getInitialDirection(initialDirection, model);
    bool success = model->getMouse()->initialize(mouseFile, direction);
    if (!success) {
        L()->error(
            "Unable to successfully initialize the mouse in the algorithm"
            " \"%v\" from \"%v\".",
            mouseAlgorithm,
            mouseFile);
        SimUtilities::quit();
    }

    // Validate the mouse
    if (STRING_TO_INTERFACE_TYPE.at(interfaceType) == InterfaceType::DISCRETE) {
        if (!MouseChecker::isDiscreteInterfaceCompatible(*model->getMouse())) {
            L()->error("The mouse file \"%v\" is not discrete interface compatible.", mouseFile);
            SimUtilities::quit();
        }
    }
    else { // InterfaceType::CONTINUOUS
        if (!MouseChecker::isContinuousInterfaceCompatible(*model->getMouse())) {
            L()->error("The mouse file \"%v\" is not continuous interface compatible.", mouseFile);
            SimUtilities::quit();
        }
    }
}

Direction Controller::getInitialDirection(const std::string& initialDirection, Model* model) {
    bool wallNorth = model->getMaze()->getTile(0, 0)->isWall(Direction::NORTH);
    bool wallEast = model->getMaze()->getTile(0, 0)->isWall(Direction::EAST);
    if (!ContainerUtilities::mapContains(STRING_TO_DIRECTION, initialDirection) && wallNorth == wallEast) {
        return Direction::NORTH;
    }
    if (initialDirection == OPENING_DIRECTION_STRING) {
        return (wallNorth ? Direction::EAST : Direction::NORTH);
    }
    if (initialDirection == WALL_DIRECTION_STRING) {
        return (wallNorth ? Direction::NORTH : Direction::EAST);
    }
    return STRING_TO_DIRECTION.at(initialDirection);
}

} // namespace sim
