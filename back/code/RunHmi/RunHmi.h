#ifndef RUNHMI_H
#define RUNHMI_H

#include "../Tool/Thread.h"
#include "../Debug/Debug.h"


class RunHmi : public Thread
{
public:
    RunHmi();
private:
    void* run(void *arg);
};

#endif // RUNHMI_H
