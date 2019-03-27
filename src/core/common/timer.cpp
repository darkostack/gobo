#include "timer.hpp"

#include "common/code_utils.hpp"
#include "common/debug.hpp"
#include "common/instance.hpp"
#include "common/logging.hpp"

namespace go {

const TimerScheduler::AlarmApi TimerMilliScheduler::sAlarmMilliApi = {
    &goPlatAlarmMilliStartAt,
    &goPlatAlarmMilliStop,
    &goPlatAlarmMilliGetNow
};

bool Timer::DoesFireBefore(const Timer &aSecondTimer, uint32_t aNow)
{
    bool retval;
    bool isBeforeNow = TimerScheduler::IsStrictlyBefore(GetFireTime(), aNow);

    // Check if one timer is before `now` and the other one is not.
    if (TimerScheduler::IsStrictlyBefore(aSecondTimer.GetFireTime(), aNow) != isBeforeNow)
    {
        // One timer is before `now` and the other one is not, so if this timer's fire time is before `now` then
        // the second fire time would be after `now` and this timer would fire before the second timer.

        retval = isBeforeNow;
    }
    else
    {
        // Both timers are before `now` or both are after `now`. Either way the difference is guaranteed to be less
        // than `kMaxDt` so we can safely compare the fire times directly.

        retval = TimerScheduler::IsStrictlyBefore(GetFireTime(), aSecondTimer.GetFireTime());
    }

    return retval;
}

void TimerMilli::StartAt(uint32_t aT0, uint32_t aDt)
{
    assert(aDt <= kMaxDt);
    mFireTime = aT0 + aDt;
    GetTimerMilliScheduler().Add(*this);
}

void TimerMilli::Stop(void)
{
    GetTimerMilliScheduler().Remove(*this);
}

TimerMilliScheduler &TimerMilli::GetTimerMilliScheduler(void) const
{
    return GetInstance().GetTimerMilliScheduler();
}

void TimerScheduler::Add(Timer &aTimer, const AlarmApi &aAlarmApi)
{
    Remove(aTimer, aAlarmApi);

    if (mHead == NULL)
    {
        mHead        = &aTimer;
        aTimer.mNext = NULL;
        SetAlarm(aAlarmApi);
    }
    else
    {
        Timer *prev = NULL;
        Timer *cur;

        for (cur = mHead; cur; cur = cur->mNext)
        {
            if (aTimer.DoesFireBefore(*cur, aAlarmApi.AlarmGetNow()))
            {
                if (prev)
                {
                    aTimer.mNext = cur;
                    prev->mNext  = &aTimer;
                }
                else
                {
                    aTimer.mNext = mHead;
                    mHead        = &aTimer;
                    SetAlarm(aAlarmApi);
                }

                break;
            }

            prev = cur;
        }

        if (cur == NULL)
        {
            prev->mNext  = &aTimer;
            aTimer.mNext = NULL;
        }
    }
}

void TimerScheduler::Remove(Timer &aTimer, const AlarmApi &aAlarmApi)
{
    VerifyOrExit(aTimer.mNext != &aTimer);

    if (mHead == &aTimer)
    {
        mHead = aTimer.mNext;
        SetAlarm(aAlarmApi);
    }
    else
    {
        for (Timer *cur = mHead; cur; cur = cur->mNext)
        {
            if (cur->mNext == &aTimer)
            {
                cur->mNext = aTimer.mNext;
                break;
            }
        }
    }

    aTimer.mNext = &aTimer;

exit:
    return;
}

void TimerScheduler::SetAlarm(const AlarmApi &aAlarmApi)
{
    if (mHead == NULL)
    {
        aAlarmApi.AlarmStop(&GetInstance());
    }
    else
    {
        uint32_t now       = aAlarmApi.AlarmGetNow();
        uint32_t remaining = IsStrictlyBefore(now, mHead->mFireTime) ? (mHead->mFireTime - now) : 0;

        aAlarmApi.AlarmStartAt(&GetInstance(), now, remaining);
    }
}

void TimerScheduler::ProcessTimers(const AlarmApi &aAlarmApi)
{
    Timer *timer = mHead;

    if (timer)
    {
        if (!IsStrictlyBefore(aAlarmApi.AlarmGetNow(), timer->mFireTime))
        {
            Remove(*timer, aAlarmApi);
            timer->Fired();
        }
        else
        {
            SetAlarm(aAlarmApi);
        }
    }
    else
    {
        SetAlarm(aAlarmApi);
    }
}

bool TimerScheduler::IsStrictlyBefore(uint32_t aTimeA, uint32_t aTimeB)
{
    uint32_t diff = aTimeA - aTimeB;

    // Three cases:
    // 1) aTimeA is before  aTimeB  =>  Difference is negative (last bit of difference is set)   => Returning true.
    // 2) aTimeA is same as aTimeB  =>  Difference is zero     (last bit of difference is clear) => Returning false.
    // 3) aTimeA is after   aTimeB  =>  Difference is positive (last bit of difference is clear) => Returning false.

    return ((diff & (1UL << 31)) != 0);
}

extern "C" void goPlatAlarmMilliFired(goInstance *aInstance)
{
    Instance *instance = static_cast<Instance *>(aInstance);

    VerifyOrExit(goInstanceIsInitialized(aInstance));
    instance->GetTimerMilliScheduler().ProcessTimers();

exit:
    return;
}

#if GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER
const TimerScheduler::AlarmApi TimerMicroScheduler::sAlarmMicroApi = {
    &goPlatAlarmMicroStartAt,
    &goPlatAlarmMicroStop,
    &goPlatAlarmMicroGetNow
};

void TimerMicro::StartAt(uint32_t aT0, uint32_t aDt)
{
    assert(aDt <= kMaxDt);
    mFireTime = aT0 + aDt;
    GetTimerMicroScheduler().Add(*this);
}

void TimerMicro::Stop(void)
{
    GetTimerMicroScheduler().Remove(*this);
}

TimerMicroScheduler &TimerMicro::GetTimerMicroScheduler(void) const
{
    return GetInstance().GetTimerMicroScheduler();
}

extern "C" void goPlatAlarmMicroFired(goInstance *aInstance)
{
    Instance *instance = static_cast<Instance *>(aInstance);

    VerifyOrExit(goInstanceIsInitialized(aInstance));
    instance->GetTimerMicroScheduler().ProcessTimers();

exit:
    return;
}
#endif // GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER

} // namespace go
