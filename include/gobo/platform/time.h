#ifndef GOBO_PLATFORM_TIME_H_
#define GOBO_PLATFORM_TIME_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t goPlatTimeGet(void);

uint16_t goPlatTimeGetXtalAccuracy(void);

#ifdef __cplusplus
}
#endif

#endif // GOBO_PLATFORM_TIME_H_
