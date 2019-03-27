#ifndef GOBO_PLATFORM_SYSTEM_H_
#define GOBO_PLATFORM_SYSTEM_H_

#include <gobo/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void goSysInit(int argc, char *argv[]);

bool goSysPseudoResetWasRequested(void);

void goSysProcessDrivers(goInstance *aInstance);

extern void goSysEventSignalPending(void);

#ifdef __cplusplus
}
#endif

#endif // GOBO_PLATFORM_SYSTEM_H_
