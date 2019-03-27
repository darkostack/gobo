#include "gobo-core-config.h"

#include <gobo/instance.h>
#include <gobo/platform/misc.h>

#include "common/instance.hpp"
#include "common/logging.hpp"
#include "common/new.hpp"

goInstance *goInstanceInitSingle(void)
{
    return &go::Instance::InitSingle();
}

bool goInstanceIsInitialized(goInstance *aInstance)
{
    go::Instance &instance = *static_cast<go::Instance *>(aInstance);

    return instance.IsInitialized();
}

void goInstanceFinalize(goInstance *aInstance)
{
    go::Instance &instance = *static_cast<go::Instance *>(aInstance);
    instance.Finalize();
}

void goInstanceReset(goInstance *aInstance)
{
    go::Instance &instance = *static_cast<go::Instance *>(aInstance);
    instance.Reset();
}

goLogLevel goGetDynamicLogLevel(goInstance *aInstance)
{
    goLogLevel logLevel;

#if GOBO_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    go::Instance &instance = *static_cast<go::Instance *>(aInstance);
    logLevel = instance.GetDynamicLogLevel();
#else
    logLevel = static_cast<goLogLevel>(GOBO_CONFIG_LOG_LEVEL);
    GO_UNUSED_VARIABLE(aInstance);
#endif

    return logLevel;
}

goError goSetDynamicLogLevel(goInstance *aInstance, goLogLevel aLogLevel)
{
    goError error = GO_ERROR_NONE;

#if GOBO_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    go::Instance &instance = *static_cast<go::Instance *>(aInstance);
    instance.SetDynamicLogLevel(aLogLevel);
#else
    error = GO_ERROR_DISABLED_FEATURE;
    GO_UNUSED_VARIABLE(aInstance);
    GO_UNUSED_VARIABLE(aLogLevel);
#endif

    return error;
}

const char *goGetVersionString(void)
{
#ifdef PLATFORM_VERSION_ATTR_PREFIX
    PLATFORM_VERSION_ATTR_PREFIX
#else
    static
#endif
    const char sVersion[] = PACKAGE_NAME "/" PACKAGE_VERSION "; " GOBO_CONFIG_PLATFORM_INFO
#if defined(__DATE__)
                                         "; " __DATE__ " " __TIME__
#endif
#ifdef PLATFORM_VERSION_ATTR_SUFFIX
                                             PLATFORM_VERSION_ATTR_SUFFIX
#endif
        ; // Trailing semicolon to end statement.

    return sVersion;
}
