CONFIG += qt
CONFIG += object_parallel_to_source

SOURCES += $$files(*.cpp, true)

# TODO: MACK - make this some sort of variable
DESTDIR     = ../../build/bin/test
MOC_DIR     = ../../build/moc/test
OBJECTS_DIR = ../../build/obj/test
RCC_DIR     = ../../build/rcc/test

# TODO: MACK multiple targets, for tests and for app
# http://dragly.org/2013/04/19/setting-up-unittest-with-qt-creator/

# TODO: MACK
# Any way to get obj/sim, obj/lib etc. instead of obj/src/lib, etc.?
