#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "gobo-core-config.h"

#include <stddef.h>
#include "utils/wrap_stdint.h"

#include <gobo/platform/alarm-micro.h>
#include <gobo/platform/alarm-milli.h>

#include "common/debug.hpp"
#include "common/locator.hpp"
#include "common/tasklet.hpp"

namespace go {

class TimerMilliScheduler;

class Timer : public InstanceLocator, public OwnerLocator
{
    friend class TimerScheduler;

public:
    enum
    {
        kMaxDt = (1UL << 31) - 1,
    };

    typedef void (*Handler)(Timer &aTimer);

    Timer(Instance &aInstance, Handler aHandler, void *aOwner)
        : InstanceLocator(aInstance)
        , OwnerLocator(aOwner)
        , mHandler(aHandler)
        , mFireTime(0)
        , mNext(this)
    {
    }

    uint32_t GetFireTime(void) const { return mFireTime; }

    bool IsRunning(void) const { return (mNext != this); }

protected:
    bool DoesFireBefore(const Timer &aTimer, uint32_t aNow);

    void Fired(void) { mHandler(*this); }

    Handler  mHandler;
    uint32_t mFireTime;
    Timer *  mNext;
};

class TimerMilli : public Timer
{
public:
    TimerMilli(Instance &aInstance, Handler aHandler, void *aOwner)
        : Timer(aInstance, aHandler, aOwner)
    {
    }

    void Start(uint32_t aDt) { StartAt(GetNow(), aDt); }

    void StartAt(uint32_t aT0, uint32_t aDt);

    void Stop(void);

    static uint32_t GetNow(void) { return goPlatAlarmMilliGetNow(); }

    static uint32_t SecToMsec(uint32_t aSeconds) { return aSeconds * 1000u; }

    static uint32_t MsecToSec(uint32_t aMilliSeconds) { return aMilliSeconds / 1000u; }

private:
    TimerMilliScheduler &GetTimerMilliScheduler(void) const;
};

class TimerScheduler : public InstanceLocator
{
    friend class Timer;

protected:
    struct AlarmApi
    {
        void (*AlarmStartAt)(goInstance *aInstance, uint32_t aT0, uint32_t aDt);
        void (*AlarmStop)(goInstance *aInstance);
        uint32_t (*AlarmGetNow)(void);
    };

    TimerScheduler(Instance &aInstance)
        : InstanceLocator(aInstance)
        , mHead(NULL)
    {
    }

    void Add(Timer &aTimer, const AlarmApi &aAlarmApi);

    void Remove(Timer &aTimer, const AlarmApi &aAlarmApi);

    void ProcessTimers(const AlarmApi &aAlarmApi);

    void SetAlarm(const AlarmApi &aAlarmApi);

    static bool IsStrictlyBefore(uint32_t aTimeA, uint32_t aTimeB);

    Timer *mHead;
};

class TimerMilliScheduler : public TimerScheduler
{
public:
    TimerMilliScheduler(Instance &aInstance)
        : TimerScheduler(aInstance)
    {
    }

    void Add(TimerMilli &aTimer) { TimerScheduler::Add(aTimer, sAlarmMilliApi); }

    void Remove(TimerMilli &aTimer) { TimerScheduler::Remove(aTimer, sAlarmMilliApi); }

    void ProcessTimers(void) { TimerScheduler::ProcessTimers(sAlarmMilliApi); }

private:
    static const AlarmApi sAlarmMilliApi;
};

#if GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER
class TimerMicroScheduler;

class TimerMicro : public Timer
{
public:
    TimerMicro(Instance &aInstance, Handler &aHandler, void *aOwner)
        : Timer(aInstance, aHandler, aOwner)
    {
    }

    void Start(uint32_t aDt) { StartAt(GetNow(), aDt); }

    void StartAt(uint32_t aT0, uint32_t aDt);

    void Stop(void);

    static uint32_t GetNow(void) { return goPlatAlarmMicroGetNow(); }

private:
    TimerMicroScheduler &GetTimerMicroScheduler(void) const;
};

class TimerMicroScheduler : public TimerScheduler
{
public:
    TimerMicroScheduler(Instance &aInstance)
        : TimerScheduler(aInstance)
    {
    }

    void Add(TimerMicro &aTimer) { TimerScheduler::Add(aTimer, sAlarmMicroApi); }

    void Remove(TimerMicro &aTimer) { TimerScheduler::Remove(aTimer, sAlarmMicroApi); }

    void ProcessTimers(void) { TimerScheduler::ProcessTimers(sAlarmMicroApi); }

private:
    static const AlarmApi sAlarmMicroApi;
};

#endif // GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER

} // namespace go

#endif // TIMER_HPP_
