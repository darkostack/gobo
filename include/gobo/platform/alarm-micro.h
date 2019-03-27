#ifndef GO_PLATFORM_ALARM_MICRO_H_
#define GO_PLATFORM_ALARM_MICRO_H_

#include <stdint.h>

#include <gobo/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void goPlatAlarmMicroStartAt(goInstance *aInstance, uint32_t aT0, uint32_t aDt);

void goPlatAlarmMicroStop(goInstance *aInstance);

uint32_t goPlatAlarmMicroGetNow(void);

extern void goPlatAlarmMicroFired(goInstance *aInstance);

#ifdef __cplusplus
}
#endif

#endif // GO_PLATFORM_ALARM_MICRO_H_
