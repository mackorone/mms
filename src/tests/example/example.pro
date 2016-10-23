QT += testlib
CONFIG += testcase
HEADERS += $$files(*.h, true)
SOURCES += $$files(*.cpp, true)

DESTDIR = build
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build
