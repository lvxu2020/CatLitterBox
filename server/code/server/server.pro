TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include    ($$PWD/Tool/Tool.pri)
include    ($$PWD/MqttMsg/MqttMsg.pri)
include    ($$PWD/Control/Control.pri)
include    ($$PWD/Register/Register.pri)

LIBS += -lpthread \
    -l paho-mqtt3c

SOURCES += \
        main.c
