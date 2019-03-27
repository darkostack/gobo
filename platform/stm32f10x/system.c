#include "platform-stm32f10x.h"

extern bool gPlatformPseudoResetWasRequested;

void goSysInit(void)
{
    if (gPlatformPseudoResetWasRequested)
    {
        gPlatformPseudoResetWasRequested = false;
        return;
    }

    // see: third_party/stm32f10x/drivers/misc.h
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    platformAlarmInit();
    platformLoggingInit();
}

bool goSysPseudoResetWasRequested(void)
{
    return gPlatformPseudoResetWasRequested;
}

void goSysDeInit(void)
{
}

void goSysProcessDrivers(goInstance *aInstance)
{
    platformUartProcess();
    platformAlarmProcess(aInstance);
}
