#include "Controller.h"

#include "Directory.h"
#include "Logging.h"
#include "Model.h"
#include "MouseChecker.h"
#include "Param.h"
#include "SimUtilities.h"
#include "units/Milliseconds.h"

namespace mms {

Controller::Controller(
    const Maze* maze,
    Mouse* mouse,
    Lens* lens,
    const QString& mouseAlgorithm) :
    m_maze(maze),
    m_mouse(mouse),
    m_lens(lens),
    m_mouseAlgorithm(mouseAlgorithm),
    m_interfaceType(InterfaceType::DISCRETE),
    m_interfaceTypeFinalized(false),
    m_process(nullptr) {
}

void Controller::init() {

    // First, connect the newTileLocationTraversed signal to a lambda that
    // clears tile fog. This ensures that the first tile's fog is always
    // cleared (the initial value of automaticallyClearFog is true). This means
    // that, if an algorithm doesn't want to automatically clear tile fog,
    // it'll have to disable tile fog and then mark the first tile as foggy.
    connect(
        Model::get(),
        &Model::newTileLocationTraversed,
        this,
        [=](int x, int y){
            if (getDynamicOptions().automaticallyClearFog) {
                m_lens->getMazeGraphic()->setTileFogginess(x, y, false);
            }
        }
    );

    // Initialize the mouse interface
    m_mouseInterface = new MouseInterface(
        m_maze,
        m_mouse,
        m_lens->getMazeGraphic(),
        this
    );
}

void Controller::start() {

    // TODO: MACK - Deduplicate with maze algo utilities

    // Check to see if there is some directory with the given name
    QString selectedMouseAlgo(m_mouseAlgorithm);
    QString mouseAlgoDir(Directory::get()->getSrcMouseAlgosDirectory());
    if (!SimUtilities::getTopLevelDirs(mouseAlgoDir).contains(selectedMouseAlgo)) {
         qCritical().noquote().nospace()
            << "\"" << m_mouseAlgorithm << "\" is not a valid mouse"
            << " algorithm.";
         SimUtilities::quit();
    }

    // Get the mouse algo directory
    QString selectedMouseAlgoPath = 
        Directory::get()->getSrcMouseAlgosDirectory() + m_mouseAlgorithm;

    // Get the files for the algorithm
    QPair<QStringList, QStringList> files =
        SimUtilities::getFiles(selectedMouseAlgoPath);
    QStringList relativePaths = files.first;
    QStringList absolutePaths = files.second;

    // TODO: MACK - make these constants, dedup some of this
    if (relativePaths.contains(QString("Main.cpp"))) {

        // Create the subprocess on which we'll execute the algorithm
        m_process = new QProcess();

        // Publish all algorithm stdout so that the UI can display it
        connect(
            m_process,
            &QProcess::readyReadStandardOutput,
            this,
            [=](){
                QString text = m_process->readAllStandardOutput();
                QStringList lines = getLines(text, &m_stdoutBuffer);
                for (const QString& line : lines) {
                    emit algoStdout(line);
                }
            }
        );

        // Process all stderr commands as appropriate
        connect(
            m_process,
            &QProcess::readyReadStandardError,
            this,
            [=](){
                QString text = m_process->readAllStandardError();
                QStringList lines = getLines(text, &m_stderrBuffer);
                for (const QString& line : lines) {
                    QString response = processCommand(line);
                    if (!response.isEmpty()) {
                        m_process->write((response + "\n").toStdString().c_str());
                    }
                }
            }
        );

        // Run
        m_process->start(selectedMouseAlgoPath + "/a.out");

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
    } 
    else {
        // Invalid files
        qCritical().noquote().nospace()
            << "No \"Main\" file found in \""
            << selectedMouseAlgoPath << "\"";
        SimUtilities::quit();
    }
}

InterfaceType Controller::getInterfaceType(bool canFinalize) {
    // Finalize the interface type the first time it's queried
    // by the MouseInterface (but not by the key-press logic)
    if (canFinalize && !m_interfaceTypeFinalized) {
        m_interfaceTypeFinalized = true;
    }
    return m_interfaceType;
}

DynamicMouseAlgorithmOptions Controller::getDynamicOptions() const {
    // The Controller object is the source of truth for the dynamic options
    return m_dynamicOptions;
}

QStringList Controller::getLines(const QString& text, QStringList* buffer) {

    // TODO: upforgrabs
    // Determine whether or not this function is perf sensitive. If so,
    // refactor this so that we're not copying QStrings between lists.

    // Separate the text by line
    QStringList parts = text.split("\n");

    // We'll return list of complete lines
    QStringList lines;

    // If the text has at least one newline character, we definitely have a
    // complete line; combine it with the contents of the buffer and append
    // it to the list of lines to be returned
    if (1 < parts.size()) {
        lines.append(buffer->join("") + parts.at(0));
        buffer->clear();
    }

    // All newline-separated parts in the text are lines
    for (int i = 1; i < parts.size() - 1; i += 1) {
        lines.append(parts.at(i));
    }

    // Store the last part of the text (empty string if the text ended
    // with newline) in the buffer, to be combined with future input
    buffer->append(parts.at(parts.size() - 1));

    return lines;
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
    else if (function == "useContinuousInterface") {
        if (m_interfaceTypeFinalized) {
            // TODO: MACK - error string here
        }
        else {
            m_interfaceType = InterfaceType::CONTINUOUS;
        }
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
        m_lens->initTileGraphicText(rows, cols);
        return ACK_STRING;
    }
    else if (function == "setWheelSpeedFraction") {
        m_mouseInterface->setWheelSpeedFraction(
            SimUtilities::strToDouble(tokens.at(1)));
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
        return QString::number(m_maze->getWidth());
    }
    else if (function == "mazeHeight") {
        return QString::number(m_maze->getHeight());
    }
    else if (function == "isOfficialMaze") {
        return QString::number(m_maze->isOfficialMaze());
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
        QString text = "";
        if (3 < tokens.size()) {
            text = tokens.at(3);
        }
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

} // namespace mms
