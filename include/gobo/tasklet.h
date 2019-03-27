#ifndef GOBO_TASKLET_H_
#define GOBO_TASKLET_H_

#include <gobo/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void goTaskletsProcess(goInstance *aInstance);

bool goTaskletsArePending(goInstance *aInstance);

extern void goTaskletsSignalPending(goInstance *aInstance);

#ifdef __cplusplus
}
#endif

#endif // GOBO_TASKLET_H_
