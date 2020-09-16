
INCLUDEPATH += /usr/local/include/ \
     -I/home/lvxu/work/dbus/include/dbus-1.0

HEADERS += \
    $$PWD/DbusReceive.h \
    $$PWD/DbusSend.h \
#    $$PWD/DbusAdapter.h


SOURCES += \
    $$PWD/DbusReceive.cpp \
    $$PWD/DbusSend.cpp \
#    $$PWD/DbusAdapter.cpp

	
