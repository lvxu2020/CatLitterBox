#include "mainwindow.h"

#include <QApplication>
#include "../LOG/LogStore.h"
#include "../Communication/RecFromAir.h"
#include "../Communication/SendToAir.h"
#include "../Communication/ReadConf.h"
#include "./Dbus/DbusReceive.h"
#include "./Beat/Beat.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogStore log;
    ReadConf_Single::instance();
    DbusReceive server;
    log.start();
    log.detach();
    Beat::instance()->start();
    Beat::instance()->detach();
    MainWindow w;
    w.show();
    RecFromAir_Single::instance()->start();
    RecFromAir_Single::instance()->detach();
    SendToAir_Single::instance()->start();
    SendToAir_Single::instance()->detach();

    return a.exec();
}
