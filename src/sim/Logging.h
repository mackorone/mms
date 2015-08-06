#pragma once

// These must be declared before we include easylogging++.h
#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_THREAD_SAFE

#include <easylogging++.h>

#define LOG_STRING "log"
#define PRINT_STRING "print"

// See http://stackoverflow.com/questions/5588855/standard-alternative-to-gccs-va-args-trick
// and http://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments

// Some hackiness here to still get file:line info *and* do printf-style printing
#define LOG_HELPER(LEVEL, MESSAGE, ...)\
sim::Logging::logger()->LEVEL("%v:%v - " MESSAGE, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(MESSAGE, ...)   LOG_HELPER(debug, MESSAGE, ##__VA_ARGS__)
#define LOG_INFO(MESSAGE, ...)    LOG_HELPER( info, MESSAGE, ##__VA_ARGS__)
#define LOG_WARN(MESSAGE, ...)    LOG_HELPER( warn, MESSAGE, ##__VA_ARGS__)
#define LOG_WARNING(MESSAGE, ...) LOG_HELPER( warn, MESSAGE, ##__VA_ARGS__)
#define LOG_ERROR(MESSAGE, ...)   LOG_HELPER(error, MESSAGE, ##__VA_ARGS__)
#define LOG_FATAL(MESSAGE, ...)   LOG_HELPER(fatal, MESSAGE, ##__VA_ARGS__)
#undef LOG
#define LOG(LEVEL, MESSAGE, ...) LOG_##LEVEL(MESSAGE, ##__VA_ARGS__)

// Printing prints (go figure) *and* logs as if LOG was called
#define PRINT_HELPER(LEVEL, MESSAGE, ...)\
sim::Logging::printer()->LEVEL(MESSAGE, ##__VA_ARGS__);\
LOG_HELPER(LEVEL, MESSAGE, ##__VA_ARGS__)
#define PRINT_DEBUG(MESSAGE, ...)   PRINT_HELPER(debug, MESSAGE, ##__VA_ARGS__)
#define PRINT_INFO(MESSAGE, ...)    PRINT_HELPER( info, MESSAGE, ##__VA_ARGS__)
#define PRINT_WARN(MESSAGE, ...)    PRINT_HELPER( warn, MESSAGE, ##__VA_ARGS__)
#define PRINT_WARNING(MESSAGE, ...) PRINT_HELPER( warn, MESSAGE, ##__VA_ARGS__)
#define PRINT_ERROR(MESSAGE, ...)   PRINT_HELPER(error, MESSAGE, ##__VA_ARGS__)
#define PRINT_FATAL(MESSAGE, ...)   PRINT_HELPER(fatal, MESSAGE, ##__VA_ARGS__)
#define PRINT(LEVEL, MESSAGE, ...) PRINT_##LEVEL(MESSAGE, ##__VA_ARGS__)

// Convenience for printing if appropriate, logging if not
#define PRINT_IF_ELSE_LOG(CONDITION, LEVEL, MESSAGE, ...)\
if (CONDITION) {\
    PRINT(LEVEL, MESSAGE, ##__VA_ARGS__);\
}\
else {\
    LOG(LEVEL, MESSAGE, ##__VA_ARGS__);\
}

namespace sim {

class Logging {

public:

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

    // A private constructor so we can't make instances...
    Logging(); // TODO: MACK - Logging = delete (do this in other places too! SimUtilties, etc.)
};

} // namespace sim
