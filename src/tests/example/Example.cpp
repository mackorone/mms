#include "Example.h"

void Example::example() {
    QString str = "Hello World";
    QCOMPARE(str.toUpper(), QString("HELLO WORLD"));
}

QTEST_MAIN(Example)
