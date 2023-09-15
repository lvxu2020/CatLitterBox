#ifndef DIALOGSCANMASK_H
#define DIALOGSCANMASK_H

#include <QDialog>

namespace Ui {
class DialogScanMask;
}

class DialogScanMask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogScanMask(QWidget *parent = nullptr);
    ~DialogScanMask();

private:
    Ui::DialogScanMask *ui;
};

#endif // DIALOGSCANMASK_H
