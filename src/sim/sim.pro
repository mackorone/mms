CONFIG += qt
CONFIG += object_parallel_to_source
CONFIG += warn_off # TODO: Turn these on

SOURCES += $$files(*.cpp, true)
SOURCES += $$files(../lib/*.cpp, true) # TODO: Remove this

HEADERS += $$files(*.h, true)
HEADERS += $$files(../lib/*.h, true) # TODO: Remove this

INCLUDEPATH += ../lib

# TODO: MACK - are all of these necessary?
LIBS += -lGLEW
LIBS += -lGL
LIBS += -lglut
LIBS += -lGLU
LIBS += -lpthread

# TODO: MACK - make this some sort of variable
DESTDIR     = ../../bin
MOC_DIR     = ../../build/moc/sim
OBJECTS_DIR = ../../build/obj/sim
RCC_DIR     = ../../build/rcc/sim

# TODO: MACK multiple targets, for tests and for app
# http://dragly.org/2013/04/19/setting-up-unittest-with-qt-creator/

# TODO: MACK
# Any way to get obj/sim, obj/lib etc. instead of obj/src/lib, etc.?
