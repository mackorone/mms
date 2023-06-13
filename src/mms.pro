QT += core
QT += gui
QT += opengl
QT += openglwidgets
QT += widgets
QT += xml

TEMPLATE = app

CONFIG += c++11
CONFIG += debug
CONFIG += object_parallel_to_source
CONFIG += qt

SOURCES += $$files(*.cpp, true)
HEADERS += $$files(*.h, true)
RESOURCES = resources.qrc

DESTDIR     = ../bin
MOC_DIR     = ../build/moc
OBJECTS_DIR = ../build/obj
RCC_DIR     = ../build/rcc
