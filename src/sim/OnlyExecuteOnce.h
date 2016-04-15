#include "Assert.h"

#define ONLY_EXECUTE_ONCE {\
    static bool alreadyExecuted = false;\
    SIM_ASSERT_FA(alreadyExecuted);\
    alreadyExecuted = true;\
}
