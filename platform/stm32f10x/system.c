#include "platform-stm32f10x.h"

extern bool gPlatformPseudoResetWasRequested;

static void ledInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void goSysInit(void)
{
    if (gPlatformPseudoResetWasRequested)
    {
        gPlatformPseudoResetWasRequested = false;
        return;
    }

    // see: third_party/stm32f10x/drivers/misc.h
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    ledInit();

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
