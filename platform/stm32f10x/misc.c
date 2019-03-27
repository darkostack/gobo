#include "platform-stm32f10x.h"

#include <unistd.h>

#include <gobo/platform/misc.h>
#include <gobo/platform/system.h>

static goPlatResetReason   sPlatResetReason = GO_PLAT_RESET_REASON_POWER_ON;
bool                       gPlatformPseudoResetWasRequested = false;
static goPlatMcuPowerState gPlatMcuPowerState = GO_PLAT_MCU_POWER_STATE_ON;

void goPlatReset(goInstance *aInstance)
{
    GO_UNUSED_VARIABLE(aInstance);

#if GOBO_PLATFORM_USE_PSEUDO_RESET
    gPlatformPseudoResetWasRequested = true;
    sPlatResetReason                 = GO_PLAT_RESET_REASON_SOFTWARE;
#else
    goSysDeInit();
    platformUartRestore();

    NVIC_SystemReset();
#endif
}

goPlatResetReason goPlatGetResetReason(goInstance *aInstance)
{
    GO_UNUSED_VARIABLE(aInstance);

    return sPlatResetReason;
}

void goPlatWakeHost(void)
{
    // TODO: implement an operation to wake the host from sleep state.
}

goError goPlatSetMcuPowerState(goInstance *aInstance, goPlatMcuPowerState aState)
{
    goError error = GO_ERROR_NONE;

    GO_UNUSED_VARIABLE(aInstance);

    switch (aState)
    {
    case GO_PLAT_MCU_POWER_STATE_ON:
    case GO_PLAT_MCU_POWER_STATE_LOW_POWER:
        gPlatMcuPowerState = aState;
        break;

    default:
        error = GO_ERROR_FAILED;
        break;
    }

    return error;
}

goPlatMcuPowerState goPlatGetMcuPowerState(goInstance *aInstance)
{
    GO_UNUSED_VARIABLE(aInstance);

    return gPlatMcuPowerState;
}
