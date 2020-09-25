TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include    ($$PWD/Debug/Debug.pri)
include    ($$PWD/Config/Config.pri)


LIBS += -lpthread \
    -l paho-mqtt3c

SOURCES += \
        main.c
