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
        m_mouseInterface(nullptr),
        m_staticOptionsFinalized(false) {

    // Start the mouse algorithm
    startMouseAlgorithm(P()->mouseAlgorithm());

    // TODO: MACK - wait until static options have been finalized
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
        model
    );

    // Initialize the mouse interface
    m_mouseInterface = new MouseInterface(
        model->getMaze(),
        model->getMouse(),
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
        m_process->write(response.toStdString().c_str());
        m_inputLines.clear();
    }

    // For all complete lines in the input, take action
    for (int i = 1; i < inputLines.size() - 1; i += 1) {
        QString response = processCommand(inputLines.at(i));
        m_process->write(response.toStdString().c_str());
    }

    // Store the beginning of the incomplete line
    m_inputLines.append(inputLines.at(inputLines.size() - 1));
}

QString Controller::processCommand(const QString& command) {
    // TODO: MACK - split the command into tokens, parse each one
    //qDebug() << command;
    if (command == "wallFront") {
        QString response = m_mouseInterface->wallFront() ? "true\n" : "false\n";
        //qDebug() << response;
        return response;
    }
    if (command == "wallRight") {
        QString response = m_mouseInterface->wallRight() ? "true\n" : "false\n";
        //qDebug() << response;
        return response;
    }
    if (command == "wallLeft") {
        QString response = m_mouseInterface->wallLeft() ? "true\n" : "false\n";
        //qDebug() << response;
        return response;
    }
    if (command == "moveForward") {
        m_mouseInterface->moveForward();
        return QString("ACK\n");
    }
    if (command == "turnLeft") {
        m_mouseInterface->turnLeft();
        return QString("ACK\n");
    }
    if (command == "turnRight") {
        m_mouseInterface->turnRight();
        return QString("ACK\n");
    }
    return QString("ERROR\n");
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
