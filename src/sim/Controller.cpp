#include "Controller.h"

#include <QCoreApplication>

#include "Directory.h"
#include "Model.h"
#include "MouseChecker.h"
#include "Param.h"
#include "SimUtilities.h"
#include "Map.h"
#include "units/Milliseconds.h"

namespace mms {

Controller::Controller(Model* model, Map* map, const QString& mouseAlgorithm) :
        m_model(model),
        m_map(map),
        m_mouseAlgorithm(mouseAlgorithm),
        m_staticOptionsFinalized(false),
        m_process(nullptr) {
}

void Controller::init() {

    startMouseAlgorithm(m_mouseAlgorithm);
    connect(
        m_process,
        &QProcess::readyReadStandardOutput,
        this,
        &Controller::processMouseAlgoStdout
    );
    connect(
        m_process,
        &QProcess::readyReadStandardError,
        this,
        &Controller::processMouseAlgoStderr
    );


    // TODO: MACK - eventually remove this
    // Wait until static options have been finalized
    while (!m_staticOptionsFinalized) {
        SimUtilities::sleep(Milliseconds(1));
        // We haven't started the main event loop yet, so we have to explicitly
        // process events so that we can detect messages from the algorithm.
        QCoreApplication::processEvents();
    }

    // Validate all of the static options except for mouseFile,
    // which is validated in the mouse init method
    validateMouseInterfaceType(
        m_mouseAlgorithm,
        m_staticOptions.interfaceType
    );

    // Initialize the mouse interface
    m_mouseInterface = new MouseInterface(
        m_model->getMaze(),
        m_model->getMouse(),
        m_map->getMazeGraphic(),
        this,
        m_map->getAllowableTileTextCharacters(),
        m_map->getBufferInterface()
    );
}

StaticMouseAlgorithmOptions Controller::getStaticOptions() {
    // The Controller object is the source of truth for the static options
    return m_staticOptions;
}

DynamicMouseAlgorithmOptions Controller::getDynamicOptions() {
    // The Controller object is the source of truth for the dynamic options
    return m_dynamicOptions;
}


void Controller::processMouseAlgoStdout() {
    QString output = m_process->readAllStandardOutput();
    for (const QString& line : output.split("\n", QString::SkipEmptyParts)) {
        // TODO: MACK - format this better, put this in the GUI, log it, etc.
        qDebug() << "ALGO:" << line;
    }
}

void Controller::processMouseAlgoStderr() {

    // TODO: upforgrabs
    // Determine whether or not this function is perf sensitive. If so,
    // refactor this so that we're not copying QStrings between lists.

    // Read all of the new text
    QString input = m_process->readAllStandardError();

    // Separate the input by line
    QStringList inputLines = input.split("\n");

    // A list of commands to be executed
    QStringList commands;

    // If a line has definitely terminated, it's a command
    if (1 < inputLines.size()) {
        commands.append(m_inputLines.join("") + inputLines.at(0));
        m_inputLines.clear();
    }

    // All complete lines in the input are commands
    for (int i = 1; i < inputLines.size() - 1; i += 1) {
        commands.append(inputLines.at(i));
    }

    // Store the beginning of the incomplete line
    m_inputLines.append(inputLines.at(inputLines.size() - 1));

    // Process all available commands
    for (int i = 0; i < commands.size(); i += 1) {
        QString response = processCommand(commands.at(i));
        if (!response.isEmpty()) {
            m_process->write((response + "\n").toStdString().c_str());
        }
    }
}

QString Controller::processCommand(const QString& command) {

    // TODO: upforgrabs
    // These functions should have sanity checks, e.g., correct
    // types, not finalizing static options more than once, etc.

    static const QString ACK_STRING = "ACK";
    static const QString EMPTY_STRING = "";
    static const QString ERROR_STRING = "!";

    QStringList tokens = command.split(" ", QString::SkipEmptyParts);
    QString function = tokens.at(0);

    // TODO: We don't need static config - we can just update the values as we get them
    // TODO: MACK - maybe just call these "update"?
    if (function == "setMouseFile") {
        // TODO: MACK - should I do something with success/failure flag?
        // TODO: MACK - call this reload
        m_mouseInterface->setMouseFile(tokens.at(1));
        return ACK_STRING;
    }
    else if (function == "setInterfaceType") {
        m_staticOptions.interfaceType = tokens.at(1);
        return ACK_STRING;
    }
    else if (function == "setInitialDirection") {
        char direction = SimUtilities::strToChar(tokens.at(1));
        m_mouseInterface->setStartingDirection(direction);
        return ACK_STRING;
    }
    else if (function == "setTileTextRowsAndCols") {
        // TODO: MACK - validation
        // if (tileTextNumberOfRows < 0 || tileTextNumberOfCols < 0) {
        //     qCritical().noquote().nospace()
        //         << "Both tileTextNumberOfRows() and tileTextNumberOfCols() must"
        //         << " return non-negative integers. Since they return \""
        //         << tileTextNumberOfRows << "\" and \"" << tileTextNumberOfCols
        //         << "\", respectively, the tile text dimensions of the mouse"
        //         << " algorithm \"" << mouseAlgorithm << "\" are invalid.";
        //     SimUtilities::quit();
        // }
        int rows = SimUtilities::strToInt(tokens.at(1));
        int cols = SimUtilities::strToInt(tokens.at(2));
        m_map->initTileGraphicText(rows, cols);
        return ACK_STRING;
    }
    else if (function == "setWheelSpeedFraction") {
        m_mouseInterface->setWheelSpeedFraction(
            SimUtilities::strToDouble(tokens.at(1)));
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
        return QString(QChar(m_mouseInterface->getStartedDirection()));
    }
    else if (function == "getRandomFloat") {
        return QString::number(m_mouseInterface->getRandom());
    }
    else if (function == "millis") {
        return QString::number(m_mouseInterface->millis());
    }
    else if (function == "delay") {
        int milliseconds = SimUtilities::strToInt(tokens.at(1));
        m_mouseInterface->delay(milliseconds);
        return ACK_STRING;
    }
    else if (function == "setTileColor") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        char color = SimUtilities::strToChar(tokens.at(3));
        m_mouseInterface->setTileColor(x, y, color);
        return EMPTY_STRING;
    }
    else if (function == "clearTileColor") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        m_mouseInterface->clearTileColor(x, y);
        return EMPTY_STRING;
    }
    else if (function == "clearAllTileColor") {
        m_mouseInterface->clearAllTileColor();
        return EMPTY_STRING;
    }
    else if (function == "setTileText") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        QString text = tokens.at(3);
        m_mouseInterface->setTileText(x, y, text);
        return EMPTY_STRING;
    }
    else if (function == "clearTileText") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        m_mouseInterface->clearTileText(x, y);
        return EMPTY_STRING;
    }
    else if (function == "clearAllTileText") {
        m_mouseInterface->clearAllTileText();
        return EMPTY_STRING;
    }
    else if (function == "declareWall") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        char direction = SimUtilities::strToChar(tokens.at(3));
        bool wallExists = SimUtilities::strToBool(tokens.at(4));
        m_mouseInterface->declareWall(x, y, direction, wallExists);
        return EMPTY_STRING;
    }
    else if (function == "undeclareWall") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        char direction = SimUtilities::strToChar(tokens.at(3));
        m_mouseInterface->undeclareWall(x, y, direction);
        return EMPTY_STRING;
    }
    else if (function == "setTileFogginess") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        bool foggy = SimUtilities::strToBool(tokens.at(3));
        m_mouseInterface->setTileFogginess(x, y, foggy);
        return EMPTY_STRING;
    }
    else if (function == "declareTileDistance") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        int distance = SimUtilities::strToInt(tokens.at(3));
        m_mouseInterface->declareTileDistance(x, y, distance);
        return EMPTY_STRING;
    }
    else if (function == "undeclareTileDistance") {
        int x = SimUtilities::strToInt(tokens.at(1));
        int y = SimUtilities::strToInt(tokens.at(2));
        m_mouseInterface->undeclareTileDistance(x, y);
        return EMPTY_STRING;
    }
    else if (function == "resetPosition") {
        m_mouseInterface->resetPosition();
        return ACK_STRING;
    }
    else if (function == "inputButtonPressed") {
        int inputButton = SimUtilities::strToInt(tokens.at(1));
        return SimUtilities::boolToStr(
            m_mouseInterface->inputButtonPressed(inputButton)
        );
    }
    else if (function == "acknowledgeInputButtonPressed") {
        int inputButton = SimUtilities::strToInt(tokens.at(1));
        m_mouseInterface->acknowledgeInputButtonPressed(inputButton);
        return ACK_STRING;
    }
    else if (function == "getWheelMaxSpeed") {
        QString name = tokens.at(1);
        return QString::number(m_mouseInterface->getWheelMaxSpeed(name));
    }
    else if (function == "setWheelSpeed") {
        QString name = tokens.at(1);
        double rpm = SimUtilities::strToDouble(tokens.at(2));
        m_mouseInterface->setWheelSpeed(name, rpm);
        return ACK_STRING;
    }
    else if (function == "getWheelEncoderTicksPerRevolution") {
        QString name = tokens.at(1);
        return QString::number(
            m_mouseInterface->getWheelEncoderTicksPerRevolution(name)
        );
    }
    else if (function == "readWheelEncoder") {
        QString name = tokens.at(1);
        return QString::number(
            m_mouseInterface->readWheelEncoder(name)
        );
    }
    else if (function == "resetWheelEncoder") {
        QString name = tokens.at(1);
        m_mouseInterface->resetWheelEncoder(name);
        return ACK_STRING;
    }
    else if (function == "readSensor") {
        QString name = tokens.at(1);
        return QString::number(m_mouseInterface->readSensor(name));
    }
    else if (function == "readGyro") {
        QString name = tokens.at(1);
        return QString::number(m_mouseInterface->readGyro());
    }
    else if (function == "wallFront") {
        return SimUtilities::boolToStr(m_mouseInterface->wallFront());
    }
    else if (function == "wallRight") {
        return SimUtilities::boolToStr(m_mouseInterface->wallRight());
    }
    else if (function == "wallLeft") {
        return SimUtilities::boolToStr(m_mouseInterface->wallLeft());
    }
    else if (function == "moveForward") {
        int count = 1;
        if (1 < tokens.size()) {
            count = SimUtilities::strToInt(tokens.at(1));
        }
        m_mouseInterface->moveForward(count);
        return ACK_STRING;
    }
    else if (function == "turnLeft") {
        m_mouseInterface->turnLeft();
        return ACK_STRING;
    }
    else if (function == "turnRight") {
        m_mouseInterface->turnRight();
        return ACK_STRING;
    }
    else if (function == "turnAroundLeft") {
        m_mouseInterface->turnAroundLeft();
        return ACK_STRING;
    }
    else if (function == "turnAroundRight") {
        m_mouseInterface->turnAroundRight();
        return ACK_STRING;
    }
    else if (function == "originMoveForwardToEdge") {
        m_mouseInterface->originMoveForwardToEdge();
        return ACK_STRING;
    }
    else if (function == "originTurnLeftInPlace") {
        m_mouseInterface->originTurnLeftInPlace();
        return ACK_STRING;
    }
    else if (function == "originTurnRightInPlace") {
        m_mouseInterface->originTurnRightInPlace();
        return ACK_STRING;
    }
    else if (function == "moveForwardToEdge") {
        int count = 1;
        if (1 < tokens.size()) {
            count = SimUtilities::strToInt(tokens.at(1));
        }
        m_mouseInterface->moveForwardToEdge(count);
        return ACK_STRING;
    }
    else if (function == "turnLeftToEdge") {
        m_mouseInterface->turnLeftToEdge();
        return ACK_STRING;
    }
    else if (function == "turnRightToEdge") {
        m_mouseInterface->turnRightToEdge();
        return ACK_STRING;
    }
    else if (function == "turnAroundLeftToEdge") {
        m_mouseInterface->turnAroundLeftToEdge();
        return ACK_STRING;
    }
    else if (function == "turnAroundRightToEdge") {
        m_mouseInterface->turnAroundRightToEdge();
        return ACK_STRING;
    }
    else if (function == "diagonalLeftLeft") {
        int count = SimUtilities::strToInt(tokens.at(1));
        m_mouseInterface->diagonalLeftLeft(count);
        return ACK_STRING;
    }
    else if (function == "diagonalLeftRight") {
        int count = SimUtilities::strToInt(tokens.at(1));
        m_mouseInterface->diagonalLeftRight(count);
        return ACK_STRING;
    }
    else if (function == "diagonalRightLeft") {
        int count = SimUtilities::strToInt(tokens.at(1));
        m_mouseInterface->diagonalRightLeft(count);
        return ACK_STRING;
    }
    else if (function == "diagonalRightRight") {
        int count = SimUtilities::strToInt(tokens.at(1));
        m_mouseInterface->diagonalRightRight(count);
        return ACK_STRING;
    }
    else if (function == "currentXTile") {
        return QString::number(m_mouseInterface->currentXTile());
    }
    else if (function == "currentYTile") {
        return QString::number(m_mouseInterface->currentYTile());
    }
    else if (function == "currentDirection") {
        return QString(QChar(m_mouseInterface->currentDirection()));
    }
    else if (function == "currentXPosMeters") {
        return QString::number(m_mouseInterface->currentXPosMeters());
    }
    else if (function == "currentYPosMeters") {
        return QString::number(m_mouseInterface->currentYPosMeters());
    }
    else if (function == "currentRotationDegrees") {
        return QString::number(m_mouseInterface->currentRotationDegrees());
    }

    return ERROR_STRING;
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

} // namespace mms
