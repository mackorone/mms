#pragma once

#include <iostream>
#include <stdexcept>

#define ERR_MSG "Assertion failed!"
#define OPENING "---------- " ERR_MSG " ----------"
#define CLOSING "---------------------------------------"

#define ASSERT_TR(condition)                         \
  if (!(condition)) {                                \
    std::cerr << OPENING << std::endl                \
              << "FILE: " << __FILE__ << std::endl   \
              << "LINE: " << __LINE__ << std::endl   \
              << "COND: " << #condition << std::endl \
              << CLOSING << std::endl;               \
    throw std::runtime_error(ERR_MSG);               \
  }

#define ASSERT_FA(condition)                            \
  if (condition) {                                      \
    std::cerr << OPENING << std::endl                   \
              << "FILE: " << __FILE__ << std::endl      \
              << "LINE: " << __LINE__ << std::endl      \
              << "COND: "                               \
              << "!(" << #condition << ")" << std::endl \
              << CLOSING << std::endl;                  \
    throw std::runtime_error(ERR_MSG);                  \
  }

#define ASSERT_EQ(lhs, rhs)                                      \
  if (!((lhs) == (rhs))) {                                       \
    std::cerr << OPENING << std::endl                            \
              << "FILE: " << __FILE__ << std::endl               \
              << "LINE: " << __LINE__ << std::endl               \
              << "COND: " << #lhs << " == " << #rhs << std::endl \
              << "LHS: " << lhs << std::endl                     \
              << "RHS: " << rhs << std::endl                     \
              << CLOSING << std::endl;                           \
    throw std::runtime_error(ERR_MSG);                           \
  }

#define ASSERT_NE(lhs, rhs)                                      \
  if (!((lhs) != (rhs))) {                                       \
    std::cerr << OPENING << std::endl                            \
              << "FILE: " << __FILE__ << std::endl               \
              << "LINE: " << __LINE__ << std::endl               \
              << "COND: " << #lhs << " != " << #rhs << std::endl \
              << "LHS: " << lhs << std::endl                     \
              << "RHS: " << rhs << std::endl                     \
              << CLOSING << std::endl;                           \
    throw std::runtime_error(ERR_MSG);                           \
  }

#define ASSERT_LT(lhs, rhs)                                     \
  if (!((lhs) < (rhs))) {                                       \
    std::cerr << OPENING << std::endl                           \
              << "FILE: " << __FILE__ << std::endl              \
              << "LINE: " << __LINE__ << std::endl              \
              << "COND: " << #lhs << " < " << #rhs << std::endl \
              << "LHS: " << lhs << std::endl                    \
              << "RHS: " << rhs << std::endl                    \
              << CLOSING << std::endl;                          \
    throw std::runtime_error(ERR_MSG);                          \
  }

#define ASSERT_LE(lhs, rhs)                                      \
  if (!((lhs) <= (rhs))) {                                       \
    std::cerr << OPENING << std::endl                            \
              << "FILE: " << __FILE__ << std::endl               \
              << "LINE: " << __LINE__ << std::endl               \
              << "COND: " << #lhs << " <= " << #rhs << std::endl \
              << "LHS: " << lhs << std::endl                     \
              << "RHS: " << rhs << std::endl                     \
              << CLOSING << std::endl;                           \
    throw std::runtime_error(ERR_MSG);                           \
  }

#define ASSERT_RUNS_AT_MOST_N_TIMES(N) \
  {                                    \
    static int count = 0;              \
    ASSERT_LT(count, N);               \
    count += 1;                        \
  }

#define ASSERT_NEVER_RUNS() ASSERT_RUNS_AT_MOST_N_TIMES(0);

#define ASSERT_RUNS_JUST_ONCE() ASSERT_RUNS_AT_MOST_N_TIMES(1);
