#pragma once

// Asserts are a simple, development-time condition-checking mechanism. They are
// meant to be used by authors of mms code and, as such, simply print to
// std::cerr. If, for some reason, this is insufficient for you, you're probably
// using them incorrectly. 

#include <iostream>

#define OPENING "---------- Assertion failed! ----------"
#define CLOSING "---------------------------------------"

#define SIM_ASSERT_TR(condition)\
if (!(condition)) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " << __FILE__ << std::endl\
              << "LINE: " << __LINE__ << std::endl\
              << "COND: " << #condition << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define SIM_ASSERT_FA(condition)\
if (condition) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " << __FILE__ << std::endl\
              << "LINE: " << __LINE__ << std::endl\
              << "COND: " << "!(" << #condition << ")" << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define SIM_ASSERT_EQ(lhs, rhs)\
if (!((lhs) == (rhs))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " << __FILE__ << std::endl\
              << "LINE: " << __LINE__ << std::endl\
              << "COND: " << #lhs << " == " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define SIM_ASSERT_NE(lhs, rhs)\
if (!((lhs) != (rhs))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " << __FILE__ << std::endl\
              << "LINE: " << __LINE__ << std::endl\
              << "COND: " << #lhs << " != " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define SIM_ASSERT_LT(lhs, rhs)\
if (!((lhs) < (rhs))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " << __FILE__ << std::endl\
              << "LINE: " << __LINE__ << std::endl\
              << "COND: " << #lhs << " < " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define SIM_ASSERT_LE(lhs, rhs)\
if (!((lhs) <= (rhs))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " << __FILE__ << std::endl\
              << "LINE: " << __LINE__ << std::endl\
              << "COND: " << #lhs << " <= " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define SIM_ASSERT_RUNS_AT_MOST_N_TIMES(N) {\
    static int count = 0;\
    SIM_ASSERT_LT(count, N);\
    count += 1;\
}

#define SIM_ASSERT_NEVER_RUNS()\
SIM_ASSERT_RUNS_AT_MOST_N_TIMES(0);

#define SIM_ASSERT_RUNS_JUST_ONCE()\
SIM_ASSERT_RUNS_AT_MOST_N_TIMES(1);
