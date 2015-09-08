#pragma once

// These must be declared before we include easylogging++.h
#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_THREAD_SAFE

#include <easyloggingpp/easylogging++.h>
#include "LoggingMacros.h"

// The purpose of LOG is to log something interally. Nothing will displayed to
// the user. The purpose of PRINT is to print something to the user as well as
// log it internally. The reasoning is that if it was shown to the user,
// someone debugging the program would want to know about it as well.
//
// To LOG or PRINT something, simply provide the level and use printf-like syntax:
//
// LOG(INFO, "Message");
// PRINT(WARN, "Message %v %v %v", 'c', "foo", 4);
//
// Note that we you should use %v for arguments of all types, and that the
// string must a string literal (no std::strings are allowed). You can write
// multiline LOG or PRINT statements as follows:
//
// PRINT(DEBUG,
//  "Message %v %v %v. This is really long and really should be on "
//  "at least two lines because it wouldn't fit on just a single line.",
//  'c', "foo", 4);

namespace sim {

class Logging {

public:

    // The logging class is not constructible
    Logging() = delete;

    // Accessors for our loggers 
    static el::Logger* logger();
    static el::Logger* printer();

    // Initializes all of the loggers, should only be called once
    static void initialize(const std::string& runId);

private:

    // Our two loggers :)
    static el::Logger* m_logger;
    static el::Logger* m_printer;

    // The default paths of our two loggers
    static std::string m_loggerPath;
    static std::string m_printerPath;

    // Used to determine part of the log file paths
    static std::string m_runId;

    // Keep track of the number of log files, so we can make more if necessary
    static int m_numLoggerFiles;
    static int m_numPrinterFiles;

    // Easy function for getting the next available log file name
    static std::string getNextFileName(const char* filename);

    // Perform an action when files get too large
    static void rolloutHandler(const char* filename, std::size_t size);
};

} // namespace sim
