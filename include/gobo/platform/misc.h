#ifndef GOBO_PLATFORM_MISC_H_
#define GOBO_PLATFORM_MISC_H_

#include <stdint.h>

#include <gobo/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void goPlatReset(goInstance *aInstance);

typedef enum {
    GO_PLAT_RESET_REASON_POWER_ON = 0,
    GO_PLAT_RESET_REASON_EXTERNAL = 1,
    GO_PLAT_RESET_REASON_SOFTWARE = 2,
    GO_PLAT_RESET_REASON_FAULT    = 3,
    GO_PLAT_RESET_REASON_CRASH    = 4,
    GO_PLAT_RESET_REASON_ASSERT   = 5,
    GO_PLAT_RESET_REASON_OTHER    = 6,
    GO_PLAT_RESET_REASON_UNKNOWN  = 7,
    GO_PLAT_RESET_REASON_WATCHDOG = 8,

    GO_PLAT_RESET_REASON_COUNT,
} goPlatResetReason;

goPlatResetReason goPlatGetResetReason(goInstance *aInstance);

void goPlatAssertFail(const char *aFilename, int aLineNumber);

void goPlatWakeHost(void);

typedef enum {
    GO_PLAT_MCU_POWER_STATE_ON        = 0,
    GO_PLAT_MCU_POWER_STATE_LOW_POWER = 1,
    GO_PLAT_MCU_POWER_STATE_OFF       = 2,
} goPlatMcuPowerState;

goError goPlatSetMcuPowerState(goInstance *aInstance, goPlatMcuPowerState aState);

goPlatMcuPowerState goPlatGetMcuPowerState(goInstance *aInstance);

#ifdef __cplusplus
}
#endif

#endif // GOBO_PLATFORM_MISC_H_
