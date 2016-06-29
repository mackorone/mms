CONFIG += qt
CONFIG += object_parallel_to_source

SOURCES += $$files(sim/*.cpp, true)
SOURCES += $$files(lib/*.cpp, true)
SOURCES += $$files(maze/*.cpp, true)

HEADERS += $$files(sim/*.h, true)
HEADERS += $$files(lib/*.h, true)
HEADERS += $$files(maze/*.h, true)

INCLUDEPATH += lib

# TODO: MACK - are all of these necessary?
LIBS += -lGLEW
LIBS += -lGL
LIBS += -lglut
LIBS += -lGLU
LIBS += -lpthread

DESTDIR = ../bin
MOC_DIR = ../moc
OBJECTS_DIR = ../obj
RCC_DIR = ../rcc

# TODO: MACK multiple targets, for tests and for app
# http://dragly.org/2013/04/19/setting-up-unittest-with-qt-creator/

# TODO: MACK
# Any way to get obj/sim, obj/lib etc. instead of obj/src/lib, etc.?
