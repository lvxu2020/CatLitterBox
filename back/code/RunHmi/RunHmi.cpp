#include "RunHmi.h"
#include <stdlib.h>

RunHmi::RunHmi()
{

}

void* RunHmi::run(void *arg)
{
    system("/home/debian/Cat/run.sh");
}
