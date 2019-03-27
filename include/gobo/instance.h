#ifndef GOBO_INSTANCE_H_
#define GOBO_INSTANCE_H_

#include <stdlib.h>

#include <gobo/error.h>
#include <gobo/platform/logging.h>
#include <gobo/platform/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct goInstance goInstance;

goInstance *goInstanceInitSingle(void);

bool goInstanceIsInitialized(goInstance *aInstance);

void goInstanceFinalize(goInstance *aInstance);

void goInstanceReset(goInstance *aInstance);

goLogLevel goGetDynamicLogLevel(goInstance *aInstance);

goError goSetDynamicLogLevel(goInstance *aInstance, goLogLevel aLogLevel);

const char *goGetVersionString(void);

#ifdef __cplusplus
}
#endif

#endif // GOBO_INSTANCE_H_
