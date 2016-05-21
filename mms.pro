CONFIG += qt
CONFIG += object_parallel_to_source
SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(src/*.h, true)

INCLUDEPATH += src/lib

LIBS += -lGLEW
LIBS += -lGL
LIBS += -lglut
LIBS += -lGLU
LIBS += -lpthread

DESTDIR = bin
MOC_DIR = moc
OBJECTS_DIR = obj
