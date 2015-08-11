#pragma once

// Returns the 64th argument
#define GET_64TH( \
 _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
_61,_62,_63,N,...) N

// This function expects 2 to 64 arguments. If 2 arguments are passed in,
// STANDARD_LOG_HELPER is returned. If 3 to 64 arguments are passed in,
// VARIADIC_LOG_HELPER is returned.
#define VLH VARIADIC_LOG_HELPER
#define GET_LOG_MACRO(...) GET_64TH(__VA_ARGS__,  \
VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, \
VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, \
VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, \
VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, \
VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, \
VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, VLH, \
VLH, STANDARD_LOG_HELPER,,)

// This function expects 2 to 64 arguments. If 2 arguments are passed in,
// STANDARD_PRINT_HELPER is returned. If 3 to 64 arguments are passed in,
// VARIADIC_PRINT_HELPER is returned.
#define VPH VARIADIC_PRINT_HELPER
#define GET_PRINT_MACRO(...) GET_64TH(__VA_ARGS__,\
VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, \
VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, \
VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, \
VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, \
VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, \
VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, VPH, \
VPH, STANDARD_PRINT_HELPER,,)

// Convert LOG LEVEL from uppercase to lowercase
#define STANDARD_LOG_DEBUG(MESSAGE) STANDARD_LOG(debug, MESSAGE)
#define STANDARD_LOG_INFO( MESSAGE) STANDARD_LOG( info, MESSAGE)
#define STANDARD_LOG_WARN( MESSAGE) STANDARD_LOG( warn, MESSAGE)
#define STANDARD_LOG_ERROR(MESSAGE) STANDARD_LOG(error, MESSAGE)
#define STANDARD_LOG_HELPER(LEVEL, MESSAGE) STANDARD_LOG_##LEVEL(MESSAGE)
#define VARIADIC_LOG_DEBUG(MESSAGE, ...) VARIADIC_LOG(debug, MESSAGE, __VA_ARGS__)
#define VARIADIC_LOG_INFO( MESSAGE, ...) VARIADIC_LOG( info, MESSAGE, __VA_ARGS__)
#define VARIADIC_LOG_WARN( MESSAGE, ...) VARIADIC_LOG( warn, MESSAGE, __VA_ARGS__)
#define VARIADIC_LOG_ERROR(MESSAGE, ...) VARIADIC_LOG(error, MESSAGE, __VA_ARGS__)
#define VARIADIC_LOG_HELPER(LEVEL, MESSAGE, ...) VARIADIC_LOG_##LEVEL(MESSAGE, __VA_ARGS__)

// Convert PRINT LEVEL from uppercase to lowercase
#define STANDARD_PRINT_DEBUG(MESSAGE) STANDARD_PRINT(debug, MESSAGE)
#define STANDARD_PRINT_INFO( MESSAGE) STANDARD_PRINT( info, MESSAGE)
#define STANDARD_PRINT_WARN( MESSAGE) STANDARD_PRINT( warn, MESSAGE)
#define STANDARD_PRINT_ERROR(MESSAGE) STANDARD_PRINT(error, MESSAGE)
#define STANDARD_PRINT_HELPER(LEVEL, MESSAGE) STANDARD_PRINT_##LEVEL(MESSAGE)
#define VARIADIC_PRINT_DEBUG(MESSAGE, ...) VARIADIC_PRINT(debug, MESSAGE, __VA_ARGS__)
#define VARIADIC_PRINT_INFO( MESSAGE, ...) VARIADIC_PRINT( info, MESSAGE, __VA_ARGS__)
#define VARIADIC_PRINT_WARN( MESSAGE, ...) VARIADIC_PRINT( warn, MESSAGE, __VA_ARGS__)
#define VARIADIC_PRINT_ERROR(MESSAGE, ...) VARIADIC_PRINT(error, MESSAGE, __VA_ARGS__)
#define VARIADIC_PRINT_HELPER(LEVEL, MESSAGE, ...) VARIADIC_PRINT_##LEVEL(MESSAGE, __VA_ARGS__)

// Convert LOG(...) to sim::Logging::logger()->level(...). Note that there is
// some hackiness here to get file:line info *and* do printf-style printing.
#define STANDARD_LOG(level, MESSAGE)\
sim::Logging::logger()->level("%v:%v - " MESSAGE, __FILE__, __LINE__)
#define VARIADIC_LOG(level, MESSAGE, ...)\
sim::Logging::logger()->level("%v:%v - " MESSAGE, __FILE__, __LINE__, __VA_ARGS__)

// Convert PRINT(...) to sim::Logging::printer()->level(...). Note that PRINT
// prints the text (go figure) *and* logs as if LOG was called.
#define STANDARD_PRINT(level, MESSAGE)\
sim::Logging::printer()->level(MESSAGE);\
STANDARD_LOG(level, MESSAGE)
#define VARIADIC_PRINT(level, MESSAGE, ...)\
sim::Logging::printer()->level(MESSAGE, __VA_ARGS__);\
VARIADIC_LOG(level, MESSAGE, __VA_ARGS__)

// Here's where the magic happens. If two arguments are passed in, we choose
// the STANDARD macro. If three or more (up to 64) arguments are passed in, we
// choose the variadic macro. This "hack" is necessary since __VA_ARGS__ cannot
// be empty, and thus we can't just always use the variadic macros. For more
// information and context, see:
// - http://stackoverflow.com/questions/5588855/standard-alternative-to-gccs-va-args-trick
// - http://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments
// - http://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#undef LOG
#define LOG(...) GET_LOG_MACRO(__VA_ARGS__)(__VA_ARGS__)
#define PRINT(...) GET_PRINT_MACRO(__VA_ARGS__)(__VA_ARGS__)

// Convenience macro for printing if appropriate, logging if not
#define IF_PRINT_ELSE_LOG(CONDITION, LEVEL, MESSAGE, ...)\
if (CONDITION) {\
    PRINT(LEVEL, MESSAGE, __VA_ARGS__);\
}\
else {\
    LOG(LEVEL, MESSAGE, __VA_ARGS__);\
}
