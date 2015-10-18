#pragma once

// ASSERT is a simple, development-time condition-checking mechanism. It is
// meant to be used by authors of mms code and, as such, simply prints to
// std::cerr. If, for some reason, this is insufficient for you, you're
// probably using it incorrectly. 

#include <iostream>

#define OPENING "---------- Assertion failed! ----------"
#define CLOSING "---------------------------------------"

#define ASSERT_TRUE(condition)\
if (!(condition)) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << #condition << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define ASSERT_FALSE(condition)\
if (condition) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << "!" << #condition << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define ASSERT_EQUAL(lhs, rhs)\
if (!((lhs) == (rhs))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << #lhs << " == " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define ASSERT_NOT_EQUAL(lhs, rhs)\
if (!((lhs) != (rhs))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << #lhs << " != " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define ASSERT_LESS_THAN(lhs, rhs)\
if (!((lhs) < (rhs))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << #lhs << " < " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define ASSERT_LESS_THAN_OR_EQUAL(lhs, rhs)\
if (!((lhs) <= (rhs))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << #lhs << " <= " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define ASSERT_IN_CLOSED_OPEN_INTERVAL(lhs, val, rhs)\
if (!(((lhs) <= (val)) && ((val) < (rhs)))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << #lhs << " <= " << #val << " < " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "VAL: " << val << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}

#define ASSERT_IN_CLOSED_CLOSED_INTERVAL(lhs, val, rhs)\
if (!(((lhs) <= (val)) && ((val) <= (rhs)))) {\
    std::cerr << OPENING << std::endl\
              << "FILE: " <<  __FILE__  << std::endl\
              << "LINE: " <<  __LINE__  << std::endl\
              << "COND: " << #lhs << " <= " << #val << " <= " << #rhs << std::endl\
              << "LHS: " << lhs << std::endl\
              << "VAL: " << val << std::endl\
              << "RHS: " << rhs << std::endl\
              << CLOSING << std::endl;\
    exit(1);\
}
