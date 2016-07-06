#include "Controller.h"

#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"

// #include "../mouse/MouseAlgorithms.h" // TODO: MACK
#include "MouseChecker.h"

namespace sim {

static const QString& OPENING_DIRECTION_STRING = "OPENING";
static const QString& WALL_DIRECTION_STRING = "WALL";

Controller::Controller(Model* model, View* view) {

    // TODO: MACK
    m_options.mouseFile = "default.xml";
    m_options.interfaceType = "DISCRETE";
    m_options.initialDirection = "NORTH";
    m_options.tileTextNumberOfRows = 2;
    m_options.tileTextNumberOfCols = 3;
    m_options.wheelSpeedFraction = 1.0;
    /*
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
    */
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

void Controller::validateMouseAlgorithm(const QString& mouseAlgorithm) {
    // TODO: MACK
    /*
    if (!MouseAlgorithms::isMouseAlgorithm(mouseAlgorithm)) {
        L()->error("\"%v\" is not a valid mouse algorithm.", mouseAlgorithm);
        SimUtilities::quit();
    }
    */
}

void Controller::validateMouseInterfaceType(
        const QString& mouseAlgorithm, const QString& interfaceType) {
    if (!STRING_TO_INTERFACE_TYPE.contains(interfaceType)) {
        L()->error(
            "\"%v\" is not a valid interface type. You must declare the "
            "interface type of the mouse algorithm \"%v\" to be either \"%v\" "
            "or \"%v\".",
            interfaceType.toStdString(),
            mouseAlgorithm.toStdString(),
            INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE).toStdString(),
            INTERFACE_TYPE_TO_STRING.value(InterfaceType::CONTINUOUS).toStdString());
        SimUtilities::quit();
    }
}

void Controller::validateMouseInitialDirection(
        const QString& mouseAlgorithm, const QString& initialDirection) {
    if (!(
        STRING_TO_DIRECTION.contains(initialDirection)
        || initialDirection == OPENING_DIRECTION_STRING
        || initialDirection == WALL_DIRECTION_STRING
    )) {
        L()->error(
            "\"%v\" is not a valid initial direction. You must declare the"
            " initial direction of the mouse algorithm \"%v\" to be one of"
            " \"%v\", \"%v\", \"%v\", \"%v\", \"%v\", or \"%v\".",
            initialDirection.toStdString(),
            mouseAlgorithm.toStdString(),
            DIRECTION_TO_STRING.value(Direction::NORTH).toStdString(),
            DIRECTION_TO_STRING.value(Direction::EAST).toStdString(),
            DIRECTION_TO_STRING.value(Direction::SOUTH).toStdString(),
            DIRECTION_TO_STRING.value(Direction::WEST).toStdString(),
            OPENING_DIRECTION_STRING.toStdString(),
            WALL_DIRECTION_STRING.toStdString());
        SimUtilities::quit();
    }
}

void Controller::validateTileTextRowsAndCols(
    const QString& mouseAlgorithm,
    int tileTextNumberOfRows, int tileTextNumberOfCols) {
    if (tileTextNumberOfRows < 0 || tileTextNumberOfCols < 0) {
        L()->error(
            "Both tileTextNumberOfRows() and tileTextNumberOfCols() must return"
            " non-negative integers. Since they return \"%v\" and \"%v\","
            " respectively, the tile text dimensions of the mouse algorithm"
            " \"%v\" are invalid.",
            tileTextNumberOfRows, 
            tileTextNumberOfCols,
            mouseAlgorithm.toStdString());
        SimUtilities::quit();
    }
}

void Controller::validateMouseWheelSpeedFraction(
    const QString& mouseAlgorithm, double wheelSpeedFraction) {
    if (!(0.0 <= wheelSpeedFraction && wheelSpeedFraction <= 1.0)) {
        L()->error(
            "\"%v\" is not a valid wheel speed fraction. The wheel speed"
            " fraction of the mouse algorithm \"%v\" has to be in [0.0, 1.0].",
            wheelSpeedFraction, 
            mouseAlgorithm.toStdString());
        SimUtilities::quit();
    }
}

void Controller::initAndValidateMouse(
        const QString& mouseAlgorithm,
        const QString& mouseFile,
        const QString& interfaceType,
        const QString& initialDirection,
        Model* model) {

    // Initialize the mouse with the file provided
    Direction direction = getInitialDirection(initialDirection, model);
    bool success = model->getMouse()->initialize(mouseFile, direction);
    if (!success) {
        L()->error(
            "Unable to successfully initialize the mouse in the algorithm"
            " \"%v\" from \"%v\".",
            mouseAlgorithm.toStdString(),
            mouseFile.toStdString());
        SimUtilities::quit();
    }

    // Validate the mouse
    if (STRING_TO_INTERFACE_TYPE.value(interfaceType) == InterfaceType::DISCRETE) {
        if (!MouseChecker::isDiscreteInterfaceCompatible(*model->getMouse())) {
            L()->error("The mouse file \"%v\" is not discrete interface compatible.", mouseFile.toStdString());
            SimUtilities::quit();
        }
    }
    else { // InterfaceType::CONTINUOUS
        if (!MouseChecker::isContinuousInterfaceCompatible(*model->getMouse())) {
            L()->error("The mouse file \"%v\" is not continuous interface compatible.", mouseFile.toStdString());
            SimUtilities::quit();
        }
    }
}

Direction Controller::getInitialDirection(const QString& initialDirection, Model* model) {
    bool wallNorth = model->getMaze()->getTile(0, 0)->isWall(Direction::NORTH);
    bool wallEast = model->getMaze()->getTile(0, 0)->isWall(Direction::EAST);
    if (!STRING_TO_DIRECTION.contains(initialDirection) && wallNorth == wallEast) {
        return Direction::NORTH;
    }
    if (initialDirection == OPENING_DIRECTION_STRING) {
        return (wallNorth ? Direction::EAST : Direction::NORTH);
    }
    if (initialDirection == WALL_DIRECTION_STRING) {
        return (wallNorth ? Direction::NORTH : Direction::EAST);
    }
    return STRING_TO_DIRECTION.value(initialDirection);
}

} // namespace sim
