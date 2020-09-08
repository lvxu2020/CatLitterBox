#TEMPLATE = lib
#CONFIG += qt warn_on
#QT -= gui
#CONFIG += c++11

#TARGET = LvXuMqtt

#INCLUDEPATH += /usr/local/include/

#LIBS += -lpthread \
##    -L /usr/local/lib/ -lpaho-mqtt3c
##    -L /home/lvxu -lpaho-mqtt3c
#    -L /home/lvxu/net/mqtt/arm_mqtt/paho.mqtt.c-master/MQinstall/lib/ -lpaho-mqtt3c

HEADERS += \
    $$PWD/mqttClinet.h \
    $$PWD/mqttSend.h


SOURCES += \
    $$PWD/mqttClinet.cpp \
    $$PWD/mqttSend.cpp

#DESTDIR =/home/lvxu/qt_output/executableProgram/cat/lib

