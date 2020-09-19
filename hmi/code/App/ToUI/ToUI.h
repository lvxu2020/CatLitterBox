#ifndef TOUI_H
#define TOUI_H

#include <QObject>
#include "../Base/singleton.h"

class ToUI : public QObject
{
    Q_OBJECT
public:
    ToUI();
    friend class Singleton<ToUI>;
signals:
    void sig_updateResault(bool res);
    void sig_updateTimerStart();
};

typedef Singleton<ToUI> ToUI_Single;    // 方便使用

#endif // TOUI_H
