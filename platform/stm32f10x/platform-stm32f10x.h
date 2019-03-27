#ifndef PLATFORM_STM32F10X_H_
#define PLATFORM_STM32F10X_H_

#include <gobo-core-config.h>

#include <gobo/instance.h>

#include "stm32f10x.h"

void platformAlarmInit(void);

void platformAlarmProcess(goInstance *aInstance);

uint64_t platformAlarmGetNow(void);

void platformLoggingInit(void);

void platformUartProcess(void);

void platformUartRestore(void);

#endif // PLATFORM_STM32F10X_H_
