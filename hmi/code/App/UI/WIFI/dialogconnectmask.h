#ifndef DIALOGCONNECTMASK_H
#define DIALOGCONNECTMASK_H

#include <QDialog>

namespace Ui {
class DialogConnectMask;
}

class DialogConnectMask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConnectMask(QWidget *parent = nullptr);
    ~DialogConnectMask();
    void setConnectStatus(QString);
private:
    Ui::DialogConnectMask *ui;
};

#endif // DIALOGCONNECTMASK_H
