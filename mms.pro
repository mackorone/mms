TEMPLATE=subdirs
SUBDIRS=src
CONFIG+=ordered


# CONFIG += qt
# CONFIG += object_parallel_to_source
# 
# SOURCES += $$files(src/*.cpp, true)
# HEADERS += $$files(src/*.h, true)
# 
# # Don't include the src/tests/ directory
# SOURCES -= $$files(src/tests/*.cpp, true)
# HEADERS -= $$files(src/tests/*.h, true)
# 
# INCLUDEPATH += src/lib
# 
# # TODO: MACK - are all of these necessary?
# LIBS += -lGLEW
# LIBS += -lGL
# LIBS += -lglut
# LIBS += -lGLU
# LIBS += -lpthread
# 
# DESTDIR = bin
# MOC_DIR = moc
# OBJECTS_DIR = obj
# RCC_DIR = rcc
# 
# # TODO: MACK multiple targets, for tests and for app
# # http://dragly.org/2013/04/19/setting-up-unittest-with-qt-creator/
# 
# # TODO: MACK
# # Any way to get obj/sim, obj/lib etc. instead of obj/src/lib, etc.?
