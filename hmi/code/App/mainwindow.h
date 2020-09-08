#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./UI/WIFI/dialogscanmask.h"
#include "./NetMonitor/netMonitor.h"
#include <vector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QListWidgetItem;
class QListWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clearQListWidget(QListWidget *,QList<QListWidgetItem*> &);
signals:
    void sig_scanWIFI();
private slots:
    void slot_scanfOver(bool);
    void slot_wifiConnectChanged(std::string);
    void slot_netStatusChange(bool);
    void on_WIFIScan_clicked();



    void on_listWIFI_clicked(const QModelIndex &index);

    void on_connectWIFI_clicked();

private:
    Ui::MainWindow *ui;
    DialogScanMask *scanMask;
    void init();

    QList<QListWidgetItem*> lstItem;
};
#endif // MAINWINDOW_H
