#include "Controller.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QProcess>

#include "Directory.h"
#include "Logging.h"
#include "MouseChecker.h"
#include "Param.h"
#include "SimUtilities.h"

namespace mms {

static const QString& OPENING_DIRECTION_STRING = "OPENING";
static const QString& WALL_DIRECTION_STRING = "WALL";

Controller::Controller(Model* model, View* view) :
        m_process(nullptr),
        m_model(model),
        m_mouseInterface(nullptr),
        m_staticOptionsFinalized(false) {

    // Start the mouse algorithm
    startMouseAlgorithm(P()->mouseAlgorithm());

    // TODO: MACK - wait until static options have been finalized
    // TODO: MACK - remove this line
    m_staticOptionsFinalized = true;
    while (!m_staticOptionsFinalized) {
        SimUtilities::sleep(Seconds(0.1));
    }

    // Validate all of the static options except for mouseFile,
    // which is validated in the mouse init method
    validateMouseInterfaceType(
        P()->mouseAlgorithm(),
        m_staticOptions.interfaceType
    );
    validateMouseInitialDirection(
        P()->mouseAlgorithm(),
        m_staticOptions.initialDirection
    );
    validateTileTextRowsAndCols(
        P()->mouseAlgorithm(),
        m_staticOptions.tileTextNumberOfRows,
        m_staticOptions.tileTextNumberOfCols
    );
    validateMouseWheelSpeedFraction(
        P()->mouseAlgorithm(),
        m_staticOptions.wheelSpeedFraction
    );

    // Initialize the mouse object
    initAndValidateMouse(
        P()->mouseAlgorithm(),
        m_staticOptions.mouseFile,
        m_staticOptions.interfaceType,
        m_staticOptions.initialDirection,
        m_model
    );

    // Initialize the mouse interface
    m_mouseInterface = new MouseInterface(
        m_model->getMaze(),
        m_model->getMouse(),
        view->getMazeGraphic(),
        this,
        view->getAllowableTileTextCharacters(),
        m_staticOptions
    );
}

MouseInterface* Controller::getMouseInterface() {
    return m_mouseInterface;
}

StaticMouseAlgorithmOptions Controller::getStaticOptions() {
    // The Controller object is the source of truth for the static options
    return m_staticOptions;
}

DynamicMouseAlgorithmOptions Controller::getDynamicOptions() {
    // The Controller object is the source of truth for the dynamic options
    return m_dynamicOptions;
}

void Controller::validateMouseInterfaceType(
        const QString& mouseAlgorithm, const QString& interfaceType) {
    if (!STRING_TO_INTERFACE_TYPE.contains(interfaceType)) {
        qCritical().noquote().nospace()
            << "\"" << interfaceType << "\" is not a valid interface type. You"
            << " must declare the interface type of the mouse algorithm \""
            << mouseAlgorithm << "\" to be either \""
            << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
            << "\" or \""
            << INTERFACE_TYPE_TO_STRING.value(InterfaceType::CONTINUOUS)
            << "\".";
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
        qCritical().noquote().nospace()
            << "\"" << initialDirection << "\" is not a valid initial"
            << " direction. You must declare the initial direction of the mouse"
            << " algorithm \"" << mouseAlgorithm << "\" to be one of \""
            << DIRECTION_TO_STRING.value(Direction::NORTH) << "\", \""
            << DIRECTION_TO_STRING.value(Direction::EAST) << "\", \""
            << DIRECTION_TO_STRING.value(Direction::SOUTH) << "\", \""
            << DIRECTION_TO_STRING.value(Direction::WEST) << "\", \""
            << OPENING_DIRECTION_STRING << "\", or \""
            << WALL_DIRECTION_STRING << "\".";
        SimUtilities::quit();
    }
}

void Controller::validateTileTextRowsAndCols(
    const QString& mouseAlgorithm,
    int tileTextNumberOfRows, int tileTextNumberOfCols) {
    if (tileTextNumberOfRows < 0 || tileTextNumberOfCols < 0) {
        qCritical().noquote().nospace()
            << "Both tileTextNumberOfRows() and tileTextNumberOfCols() must"
            << " return non-negative integers. Since they return \""
            << tileTextNumberOfRows << "\" and \"" << tileTextNumberOfCols
            << "\", respectively, the tile text dimensions of the mouse"
            << " algorithm \"" << mouseAlgorithm << "\" are invalid.";
        SimUtilities::quit();
    }
}

void Controller::validateMouseWheelSpeedFraction(
    const QString& mouseAlgorithm, double wheelSpeedFraction) {
    if (!(0.0 <= wheelSpeedFraction && wheelSpeedFraction <= 1.0)) {
        qCritical().noquote().nospace()
            << "\"" << wheelSpeedFraction << "\" is not a valid wheel speed"
            << " fraction. The wheel speed fraction of the mouse algorithm \""
            << mouseAlgorithm << "\" has to be in [0.0, 1.0].";
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
        qCritical().noquote().nospace()
            << "Unable to successfully initialize the mouse in the algorithm \""
            << mouseAlgorithm << "\" from \"" << mouseFile << "\".";
        SimUtilities::quit();
    }

    // Validate the mouse
    if (STRING_TO_INTERFACE_TYPE.value(interfaceType) == InterfaceType::DISCRETE) {
        if (!MouseChecker::isDiscreteInterfaceCompatible(*model->getMouse())) {
            qCritical().noquote().nospace()
                << "The mouse file \"" << mouseFile << "\" is not discrete"
                << " interface compatible.";
            SimUtilities::quit();
        }
    }
    else { // InterfaceType::CONTINUOUS
        if (!MouseChecker::isContinuousInterfaceCompatible(*model->getMouse())) {
            qCritical().noquote().nospace()
                << "The mouse file \"" << mouseFile << "\" is not continuous"
                << " interface compatible.";
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

void Controller::processMouseAlgoStderr() {

    // TODO: MACK - I think this is perf sensitive. If not, I should refactor
    // this so that there's only one place where we call processCommand

    // Read all of the new text
    QString input = m_process->readAllStandardError();

    // Separate the input by line
    QStringList inputLines = input.split("\n");

    // If a line has definitely terminated, take action
    if (1 < inputLines.size()) {
        QString line = m_inputLines.join("") + inputLines.at(0);
        QString response = processCommand(line);
        m_process->write((response + "\n").toStdString().c_str());
        m_inputLines.clear();
    }

    // For all complete lines in the input, take action
    for (int i = 1; i < inputLines.size() - 1; i += 1) {
        QString response = processCommand(inputLines.at(i));
        m_process->write((response + "\n").toStdString().c_str());
    }

    // Store the beginning of the incomplete line
    m_inputLines.append(inputLines.at(inputLines.size() - 1));
}

QString Controller::processCommand(const QString& command) {

    // TODO: MACK
    qDebug() << command;

    // TODO: upforgrabs
    // These functions should have sanity checks, e.g., correct
    // types, not finalizing static options more than once, etc.

    static const QString ACK_STRING = "ACK";
    static const QString ERROR_STRING = "!";

    QStringList tokens = command.split(" ", QString::SkipEmptyParts);
    QString function = tokens.at(0);

    if (function == "setMouseFile") {
        m_staticOptions.mouseFile = tokens.at(1);
        return ACK_STRING;
    }
    else if (function == "setInterfaceType") {
        m_staticOptions.interfaceType = tokens.at(1);
        return ACK_STRING;
    }
    else if (function == "setInitialDirection") {
        m_staticOptions.initialDirection = tokens.at(1);
        return ACK_STRING;
    }
    else if (function == "setTileTextNumberOfRows") {
        m_staticOptions.tileTextNumberOfRows =
            SimUtilities::strToInt(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "setTileTextNumberOfCols") {
        m_staticOptions.tileTextNumberOfCols =
            SimUtilities::strToInt(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "setWheelSpeedFraction") {
        m_staticOptions.tileTextNumberOfCols =
            SimUtilities::strToDouble(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "finalizeStaticOptions") {
        m_staticOptionsFinalized = true;
        return ACK_STRING;
    }
    else if (function == "updateAllowOmniscience") {
        m_dynamicOptions.allowOmniscience =
            SimUtilities::strToBool(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "updateAutomaticallyClearFog") {
        m_dynamicOptions.automaticallyClearFog =
            SimUtilities::strToBool(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "updateDeclareBothWallHalves") {
        m_dynamicOptions.declareBothWallHalves =
            SimUtilities::strToBool(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "updateSetTileTextWhenDistanceDeclared") {
        m_dynamicOptions.setTileTextWhenDistanceDeclared =
            SimUtilities::strToBool(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "updateSetTileBaseColorWhenDistanceDeclaredCorrectly") {
        m_dynamicOptions.setTileBaseColorWhenDistanceDeclaredCorrectly =
            SimUtilities::strToBool(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "updateDeclareWallOnRead") {
        m_dynamicOptions.declareWallOnRead =
            SimUtilities::strToBool(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "updateUseTileEdgeMovements") {
        m_dynamicOptions.useTileEdgeMovements =
            SimUtilities::strToBool(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "mazeWidth") {
        return QString::number(m_model->getMaze()->getWidth());
    }
    else if (function == "mazeHeight") {
        return QString::number(m_model->getMaze()->getHeight());
    }
    else if (function == "isOfficialMaze") {
        return QString::number(m_model->getMaze()->isOfficialMaze());
    }
    else if (function == "initialDirection") {
        // TODO: MACK
    }
    else if (function == "getRandomFloat") {
        return QString::number(m_mouseInterface->getRandom());
    }
    else if (function == "millis") {
        return QString::number(m_mouseInterface->millis());
    }
    else if (function == "delay") {
        //(int milliseconds); // # of milliseconds of sim time (adjusted based on sim speed)
    }
    else if (function == "setTileColor") {
        //(int x, int y, char color);
    }
    else if (function == "clearTileColor") {
        //(int x, int y);
    }
    else if (function == "clearAllTileColor") {
        //();
    }
    else if (function == "setTileText") {
        //(int x, int y, const std::string& text);
    }
    else if (function == "clearTileText") {
        //(int x, int y);
    }
    else if (function == "clearAllTileText") {
        //();
    }
    else if (function == "declareWall") {
        //(int x, int y, char direction, bool wallExists);
    }
    else if (function == "undeclareWall") {
        //(int x, int y, char direction);
    }
    else if (function == "setTileFogginess") {
        //(int x, int y, bool foggy);
    }
    else if (function == "declareTileDistance") {
        //(int x, int y, int distance);
    }
    else if (function == "undeclareTileDistance") {
        //(int x, int y);
    }
    else if (function == "resetPosition") {
        //();
    }
    else if (function == "inputButtonPressed") {
        //(int inputButton);
    }
    else if (function == "acknowledgeInputButtonPressed") {
        //(int inputButton);
    }
    else if (function == "getWheelMaxSpeed") {
        //(const std::string& name);
    }
    else if (function == "setWheelSpeed") {
        //(const std::string& name, double rpm);
    }
    else if (function == "getWheelEncoderTicksPerRevolution") {
        //(const std::string& name);
    }
    else if (function == "readWheelEncoder") {
        //(const std::string& name);
    }
    else if (function == "resetWheelEncoder") {
        //(const std::string& name);
    }
    else if (function == "readSensor") {
        //(const std::string& name);
    }
    else if (function == "readGyro") {
        //();
    }
    else if (function == "wallFront") {
        return m_mouseInterface->wallFront() ? "true" : "false";
    }
    else if (function == "wallRight") {
        return m_mouseInterface->wallRight() ? "true" : "false";
    }
    else if (function == "wallLeft") {
        return m_mouseInterface->wallLeft() ? "true" : "false";
    }
    else if (function == "moveForward") {
        //// TODO: MACK
        //(int count);
        m_mouseInterface->moveForward();
        return "ack";
    }
    else if (function == "turnLeft") {
        m_mouseInterface->turnLeft();
        return QString("ack");
    }
    else if (function == "turnRight") {
        m_mouseInterface->turnRight();
        return QString("ack");
    }
    else if (function == "turnRight") {
        //();
    }
    else if (function == "turnAroundLeft") {
        //();
    }
    else if (function == "turnAroundRight") {
        //();
    }
    else if (function == "originMoveForwardToEdge") {
        //();
    }
    else if (function == "originTurnLeftInPlace") {
        //();
    }
    else if (function == "originTurnRightInPlace") {
        //();
    }
    else if (function == "moveForwardToEdge") {
        //();
    }
    else if (function == "moveForwardToEdge") {
        //(int count);
    }
    else if (function == "turnLeftToEdge") {
        //();
    }
    else if (function == "turnRightToEdge") {
        //();
    }
    else if (function == "turnAroundLeftToEdge") {
        //();
    }
    else if (function == "turnAroundRightToEdge") {
        //();
    }
    else if (function == "diagonalLeftLeft") {
        //(int count);
    }
    else if (function == "diagonalLeftRight") {
        //(int count);
    }
    else if (function == "diagonalRightLeft") {
        //(int count);
    }
    else if (function == "diagonalRightRight") {
        //(int count);
    }
    else if (function == "currentXTile") {
        //();
    }
    else if (function == "currentYTile") {
        //();
    }
    else if (function == "currentDirection") {
        //();
    }
    else if (function == "currentXPosMeters") {
        //();
    }
    else if (function == "currentYPosMeters") {
        //();
    }
    else if (function == "currentRotationDegrees") {
        //();
    }
    else {
        return ERROR_STRING;
    }
}

        
void Controller::startMouseAlgorithm(const QString& mouseAlgorithm) {

    // Check to see if there is some directory with the given name
    QString selectedMouseAlgo(mouseAlgorithm);
    QString mouseAlgoDir(Directory::get()->getSrcMouseAlgosDirectory());
    if (!SimUtilities::getTopLevelDirs(mouseAlgoDir).contains(selectedMouseAlgo)) {
         qCritical().noquote().nospace()
            << "\"" << mouseAlgorithm << "\" is not a valid mouse"
            << " algorithm.";
         SimUtilities::quit();
    }

    // Get the maze algo directory
    QString selectedMouseAlgoPath = 
        Directory::get()->getSrcMouseAlgosDirectory() + mouseAlgorithm;

    // Get the files for the algorithm
    QPair<QStringList, QStringList> files =
        SimUtilities::getFiles(selectedMouseAlgoPath);
    QStringList relativePaths = files.first;
    QStringList absolutePaths = files.second;

    // TODO: MACK - make these constants, dedup some of this
    if (relativePaths.contains(QString("Main.cpp"))) {

        QString binPath = selectedMouseAlgoPath + "/a.out";

        // Build
        QStringList buildArgs = absolutePaths.filter(".cpp");
        buildArgs << "-g";
        buildArgs << "-o";
        buildArgs << binPath;
        QProcess buildProcess;
        buildProcess.start("g++", buildArgs);
        buildProcess.waitForFinished();
        if (buildProcess.exitCode() != 0) {
            qCritical().noquote()
                << "Failed to build mouse algo!"
                << "\n\n" + buildProcess.readAllStandardError();
            SimUtilities::quit();
        }

        // Run
        m_process = new QProcess();
        connect(
            m_process,
            SIGNAL(readyReadStandardError()),
            this,
            SLOT(processMouseAlgoStderr())
        );
        m_process->start(binPath);

        // TODO: MACK - use these instead of waiting for the process to finish
        // void errorOccurred(QProcess::ProcessError error)
        // void finished(int exitCode, QProcess::ExitStatus exitStatus)
        // m_process->waitForFinished();
        // if (m_process->exitCode() != 0) {
        //     qCritical().noquote()
        //         << "Mouse algo crashed!"
        //         << "\n\n" + m_process->readAllStandardError();
        //     SimUtilities::quit();
        // }
    
        // Success
        return;
    } 

    // Invalid files
    qCritical().noquote().nospace()
        << "No \"Main\" file found in \""
        << selectedMouseAlgoPath << "\"";
    SimUtilities::quit();
}
        
} // namespace mms
