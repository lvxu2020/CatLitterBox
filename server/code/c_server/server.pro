TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include    ($$PWD/Debug/Debug.pri)
include    ($$PWD/Config/Config.pri)
include    ($$PWD/Register/Register.pri)
include    ($$PWD/Control/Control.pri)
include    ($$PWD/Task/Task.pri)
include    ($$PWD/Mqtt/Mqtt.pri)


LIBS += -lpthread \
    -l paho-mqtt3c

SOURCES += \
        main.c
