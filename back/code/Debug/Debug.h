#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

#define DEBUG_POS printf(" -->%s:%d: [%s]\n", __FILE__,__LINE__,__FUNCTION__);

#define ENABLE_DEBUG

#if defined ENABLE_DEBUG
#define DEBUG(format, ...) { printf("back: "#format, ##__VA_ARGS__); DEBUG_POS }
#else
#define DEBUG(format, ...)
#endif

#endif
