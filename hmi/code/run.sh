#! /bin/sh

type devscan

if [ $? -eq 0 ]; then
    eventx=$(devscan "goodix-ts")
    echo "eventx=$eventx"
    if [ "$eventx " != " " ]; then
        if [ ! -f "/etc/pointercal" ]; then
            type ts_calibrate
            if [ $? -eq 0 ]; then
                ts_calibrate
            fi
        fi
    else 
        echo "eventx is null"
    fi
else
    echo "please install devscan"
    echo
    echo "sudo apt-get install devscan"
    exit
fi

export APP_DIR=/home/debian/cat
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/arm-linux-gnueabihf/qt5/plugins/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$APP_DIR/libskin:$APP_DIR/libqui:$APP_DIR/libffmpeg:/usr/lib:/lib
export QT_QPA_FONTDIR=/usr/share/fonts/SourceHanSans
export PATH=$PATH:$QT_DIR/libexec
export QT_QPA_PLATFORM=linuxfb:fb=/dev/fb0
export TSLIB_CONFFILE=/etc/ts.conf
export TSLIB_CALIBFILE=/etc/pointercal
export QT_QPA_GENERIC_PLUGINS=tslib:/dev/input/$eventx
export QWS_MOUSE_PROTO=tslib
export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=/dev/input/$eventx:rotate=180:invertx
cp ./lib/libBase.so.1.0.0 /usr/lib/libBase.so.1
sync

echo "start app..."

$APP_DIR/App

