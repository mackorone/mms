QT += core
QT += gui
QT += xml
QT += widgets

TEMPLATE = app

CONFIG += debug
CONFIG += object_parallel_to_source
CONFIG += qt

# TODO: upforgrabs
# Turn these on - fix a bunch of warnings
CONFIG += warn_off

SOURCES += $$files(*.cpp, true)
HEADERS += $$files(*.h, true)
RESOURCES = images.qrc

DESTDIR     = ../../bin
MOC_DIR     = ../../build/moc/sim
OBJECTS_DIR = ../../build/obj/sim
RCC_DIR     = ../../build/rcc/sim
