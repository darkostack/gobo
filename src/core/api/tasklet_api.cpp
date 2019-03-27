#include "gobo-core-config.h"

#include <gobo/tasklet.h>

#include "common/code_utils.hpp"
#include "common/instance.hpp"
#include "common/logging.hpp"

void goTaskletsProcess(goInstance *aInstance)
{
    go::Instance &instance = *static_cast<go::Instance *>(aInstance);

    VerifyOrExit(goInstanceIsInitialized(aInstance));
    instance.GetTaskletScheduler().ProcessQueuedTasklets();

exit:
    return;
}

bool goTaskletsArePending(goInstance *aInstance)
{
    bool retval = false;
    go::Instance &instance = *static_cast<go::Instance *>(aInstance);

    VerifyOrExit(goInstanceIsInitialized(aInstance));
    retval = instance.GetTaskletScheduler().AreTaskletsPending();

exit:
    return retval;
}

GO_TOOL_WEAK void goTaskletsSignalPending(goInstance *)
{
}
