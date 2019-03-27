#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

#include "gobo-core-config.h"

#include "utils/wrap_stdbool.h"
#include "utils/wrap_stdint.h"

#include <gobo/error.h>
#include <gobo/platform/logging.h>

#include "utils/heap.hpp"

#include "common/code_utils.hpp"
#include "common/timer.hpp"

typedef struct goInstance
{
} goInstance;

namespace go {

class Instance : public goInstance
{
public:
    static Instance &InitSingle(void);

    static Instance &Get(void);

    bool IsInitialized(void) const { return mIsInitialized; }

    void Reset(void);

    TimerMilliScheduler &GetTimerMilliScheduler(void) { return mTimerMilliScheduler; }

#if GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER
    TimerMicroScheduler &GetTimerMicroScheduler(void) { return mTimerMicroScheduler; }
#endif

    TaskletScheduler &GetTaskletScheduler(void) { return mTaskletScheduler; }

#if GOBO_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    goLogLevel GetDynamicLogLevel(void) const { return mLogLevel; }

    void SetDynamicLogLevel(goLogLevel aLogLevel) { mLogLevel = aLogLevel; }
#endif

    goLogLevel GetLogLevel(void) const
#if GOBO_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    {
        return GetDynamicLogLevel();
    }
#else
    {
        return static_cast<goLogLevel>(GOBO_CONFIG_LOG_LEVEL);
    }
#endif

    void Finalize(void);

#if !GOBO_ENABLE_MULTIPLE_INSTANCES
    Utils::Heap &GetHeap(void) { return mHeap; }
#endif

    template <typename Type> inline Type &Get(void);

private:
    Instance(void);
    void AfterInit(void);

    TimerMilliScheduler mTimerMilliScheduler;
#if GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER
    TimerMicroScheduler mTimerMicroScheduler;
#endif
    TaskletScheduler mTaskletScheduler;

    Utils::Heap     mHeap;

#if GOBO_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    goLogLevel mLogLevel;
#endif

    bool mIsInitialized;
};

// Specializations of the `Get<Type>()` method.

template <> inline TaskletScheduler &Instance::Get(void)
{
    return GetTaskletScheduler();
}

} // namespace go

#endif // INSTANCE_HPP_
