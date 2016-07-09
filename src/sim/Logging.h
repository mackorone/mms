#pragma once

// These must be declared before we include easylogging++.h
#define ELPP_STL_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING

#include <easyloggingpp/easylogging++.h>
#include <QMap>
#include <QPair>
#include <QString>

// To print and log something, simply provide the level and use printf-like syntax:
//
// Logging::get()->info("Message");
// Logging::get()->warn("Message %v %v %v", 'c', "foo", 4);
//
// Note that we you should use %v for arguments of all types, and that the
// string must a string literal (no QStrings are allowed). You can write
// multi-line logging statements as follows:
//
// Logging::get()->debug(
//     "Message %v %v %v. This is really long and really should be on"
//     " at least two lines because it wouldn't fit on just a single line.",
//     'c',
//      "foo",
//      4
// );
//
// The valid methods on the logger are debug, info, warn, and error. There are
// others, but those are the main ones you should use.

namespace sim {

class Logging {

public:

    // Initializes all of the loggers, should only be called once
    static void init(const QString& runId);

    // Accessor for the sim logger
    static el::Logger* get();

    // TODO: MACK
    // Accessor for the Logging object
    static Logging* getLogging();

private:

    // A private constructor is used to ensure
    // only one instance of this class exists
    Logging(const QString& runId);

    // A pointer to the actual instance of the class
    static Logging* INSTANCE;

    // Used to determine part of the log file paths
    QString m_runId;

    // The names of each of our loggers
    QString m_simLoggerName;
    QString m_mazeLoggerName;
    QString m_mouseLoggerName;

    // A map of (loggerName) -> (path, numLogFiles)
    QMap<QString, QPair<QString, int>> m_info;

    // Helper method for retrieving a particular logger
    el::Logger* getLogger(const QString& loggerName);

    // Easy function for getting the next available log file name
    QString getNextFileName(const char* filename);

    // Perform an action when files get too large
    static void rolloutHandler(const char* filename, std::size_t size);
};

} // namespace sim
