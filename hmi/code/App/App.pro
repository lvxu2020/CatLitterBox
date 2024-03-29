QT       += core gui
QT += dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#TARGET = Appb
TEMPLATE = app
CONFIG += c++11
TARGET = App
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \

FORMS += \
    mainwindow.ui

include    ($$PWD/WIFI/WIFI.pri)
include    ($$PWD/UI/UI.pri)
include    ($$PWD/../Base/Base.pro)
include    ($$PWD/../LOG/LOG.pro)
include    ($$PWD/../Mqtt/Mqtt.pro)
include    ($$PWD/NetMonitor/NetMonitor.pri)
include    ($$PWD/Json/Json.pri)
include    ($$PWD/Dbus/Dbus.pri)
include    ($$PWD/Beat/Beat.pri)
include    ($$PWD/ToUI/ToUI.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += /usr/local/include/ \




LIBS += -lpthread \
    -L/home/lvxu/net/mqtt/paho.mqtt.c/build_arm/src -lpaho-mqtt3c \





DESTDIR = /home/lvxu/outPut/exe




