CONFIG += qt
CONFIG += object_parallel_to_source
SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(src/*.h, true)

DESTDIR=qmake_bin
OBJECTS_DIR=qmake_obj
MOC_DIR=qmake_moc

INCLUDEPATH += src/lib

# message($$INCLUDEPATH)

LIBS += -lGLEW
LIBS += -lGL
LIBS += -lglut
LIBS += -lGLU
LIBS += -lpthread
