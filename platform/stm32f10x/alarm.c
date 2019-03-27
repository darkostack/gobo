#include "platform-stm32f10x.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <gobo/platform/alarm-micro.h>
#include <gobo/platform/alarm-milli.h>

#define MS_PER_S 1000
#define US_PER_MS 1000
#define US_PER_S 1000000

#define DEFAULT_TIMEOUT 10 // seconds

static bool sIsMsRunning = false;
static uint32_t sMsAlarm = 0;

static bool sIsUsRunning = false;
static uint32_t sUsAlarm = 0;

static uint64_t sSysTickMsCount = 0;

void platformAlarmInit(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1);
    }
}

uint64_t platformAlarmGetNow(void)
{
    return (uint64_t)((sSysTickMsCount * 1000) + 100 - (SysTick->VAL / 72));
}

uint32_t goPlatAlarmMilliGetNow(void)
{
    return (uint32_t)(platformAlarmGetNow() / US_PER_MS);
}

void goPlatAlarmMilliStartAt(goInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    GO_UNUSED_VARIABLE(aInstance);

    sMsAlarm = aT0 + aDt;
    sIsMsRunning = true;
}

void goPlatAlarmMilliStop(goInstance *aInstance)
{
    GO_UNUSED_VARIABLE(aInstance);

    sIsMsRunning = false;
}

uint32_t goPlatAlarmMicroGetNow(void)
{
    return (uint32_t)platformAlarmGetNow();
}

void goPlatAlarmMicroStartAt(goInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    GO_UNUSED_VARIABLE(aInstance);

    sUsAlarm = aT0 + aDt;
    sIsUsRunning = true;
}

void goPlatAlarmMicroStop(goInstance *aInstance)
{
    GO_UNUSED_VARIABLE(aInstance);

    sIsUsRunning = false;
}

void platformAlarmProcess(goInstance *aInstance)
{
    int32_t remaining;

    if (sIsMsRunning)
    {
        remaining = (int32_t)(sMsAlarm - goPlatAlarmMilliGetNow());

        if (remaining <= 0)
        {
            sIsMsRunning = false;

            goPlatAlarmMilliFired(aInstance);
        }
    }

#if GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER

    if (sIsUsRunning)
    {
        remaining = (int32_t)(sUsAlarm - goPlatAlarmMicroGetNow());

        if (remaining <= 0)
        {
            sIsUsRunning = false;

            goPlatAlarmMicroFired(aInstance);
        }
    }

#endif // GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER
}

uint64_t goPlatTimeGet(void)
{
    return platformAlarmGetNow();
}

uint16_t goPlatTimeGetXtalAccuracy(void)
{
    return 0;
}

void SysTick_Handler(void)
{
    sSysTickMsCount++;
}
