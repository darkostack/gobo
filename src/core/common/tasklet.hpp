#ifndef TASKLET_HPP_
#define TASKLET_HPP_

#include "gobo-core-config.h"

#include <stdio.h>

#include <gobo/error.h>
#include <gobo/tasklet.h>

#include "common/locator.hpp"

namespace go {

class TaskletScheduler;

class Tasklet : public InstanceLocator, public OwnerLocator
{
    friend class TaskletScheduler;

public:
    typedef void (*Handler)(Tasklet &aTasklet);

    Tasklet(Instance &aInstance, Handler aHandler, void *aOwner);

    goError Post(void);

private:
    void RunTask(void) { mHandler(*this); }

    Handler  mHandler;
    Tasklet *mNext;
};

class TaskletScheduler
{
public:
    TaskletScheduler(void);

    goError Post(Tasklet &aTasklet);

    bool AreTaskletsPending(void) { return mHead != NULL; }

    void ProcessQueuedTasklets(void);

private:
    Tasklet *PopTasklet(void);
    Tasklet *mHead;
    Tasklet *mTail;
};

} // namespace go

#endif // TASKLET_HPP_
