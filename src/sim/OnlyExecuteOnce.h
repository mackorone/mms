#include "Assert.h"

#define ONLY_EXECUTE_ONCE {\
    static bool alreadyExecuted = false;\
    ASSERT_FA(alreadyExecuted);\
    alreadyExecuted = true;\
}
