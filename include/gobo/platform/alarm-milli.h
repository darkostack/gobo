#ifndef GO_PLATFORM_ALARM_MILLI_H_
#define GO_PLATFORM_ALARM_MILLI_H_

#include <stdint.h>

#include <gobo/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void goPlatAlarmMilliStartAt(goInstance *aInstance, uint32_t aT0, uint32_t aDt);

void goPlatAlarmMilliStop(goInstance *aInstance);

uint32_t goPlatAlarmMilliGetNow(void);

extern void goPlatAlarmMilliFired(goInstance *aInstance);

#ifdef __cplusplus
}
#endif

#endif // GO_PLATFORM_ALARM_MILLI_H_
