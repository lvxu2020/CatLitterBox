#ifndef UPDATE_H
#define UPDATE_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class Update;
}

class Update : public QDialog
{
    Q_OBJECT

public:
    explicit Update(QWidget *parent = nullptr);
    ~Update();

private:
    Ui::Update *ui;
    QTimer* update;
private slots:
    void slot_updateResault(bool);
    void slot_shutDowd();
    void slot_shutTimerStart();
};

#endif // UPDATE_H
