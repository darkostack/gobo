#ifndef LOGGING_HPP_
#define LOGGING_HPP_

#include "gobo-core-config.h"

#include <ctype.h>
#include <stdio.h>
#include "utils/wrap_string.h"

#include <gobo/instance.h>
#include <gobo/platform/logging.h>

#ifdef __cplusplus
extern "C" {
#endif

#if GOBO_CONFIG_LOG_PREPEND_LEVEL
#define _GO_LEVEL_NONE_PREFIX "[NONE]"
#define _GO_LEVEL_CRIT_PREFIX "[CRIT]"
#define _GO_LEVEL_WARN_PREFIX "[WARN]"
#define _GO_LEVEL_NOTE_PREFIX "[NOTE]"
#define _GO_LEVEL_INFO_PREFIX "[INFO]"
#define _GO_LEVEL_DEBG_PREFIX "[DEBG]"
#define _GO_REGION_SUFFIX ": "
#else
#define _GO_LEVEL_NONE_PREFIX ""
#define _GO_LEVEL_CRIT_PREFIX ""
#define _GO_LEVEL_WARN_PREFIX ""
#define _GO_LEVEL_NOTE_PREFIX ""
#define _GO_LEVEL_INFO_PREFIX ""
#define _GO_LEVEL_DEBG_PREFIX ""
#define _GO_REGION_SUFFIX
#endif

#if GOBO_CONFIG_LOG_PREPEND_REGION
#define _GO_REGION_API_PREFIX "-API-----: "
#define _GO_REGION_MEM_PREFIX "-MEM-----: "
#define _GO_REGION_PLATFORM_PREFIX "-PLAT----: "
#define _GO_REGION_CLI_PREFIX "-CLI-----: "
#define _GO_REGION_CORE_PREFIX "-CORE----: "
#define _GO_REGION_UTIL_PREFIX "-UTIL----: "
#else
#define _GO_REGION_API_PREFIX _GO_REGION_SUFFIX
#define _GO_REGION_PLATFORM_PREFIX _GO_REGION_SUFFIX
#define _GO_REGION_CLI_PREFIX _GO_REGION_SUFFIX
#define _GO_REGION_CORE_PREFIX _GO_REGION_SUFFIX
#define _GO_REGION_UTIL_PREFIX _GO_REGION_SUFFIX
#endif

// Logging at level critical

#if GOBO_CONFIG_LOG_LEVEL >= GO_LOG_LEVEL_CRIT
#define goLogCrit(aInstance, aRegion, aFormat, ...) \
    _goLogFormatter(aInstance, GO_LOG_LEVEL_CRIT, aRegion, _GO_LEVEL_CRIT_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogCrit(aInstance, aRegion, aFormat, ...)
#endif

// Logging at level warning

#if GOBO_CONFIG_LOG_LEVEL >= GO_LOG_LEVEL_WARN
#define goLogWarn(aInstance, aRegion, aFormat, ...) \
    _goLogFormatter(aInstance, GO_LOG_LEVEL_WARN, aRegion, _GO_LEVEL_WARN_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogWarn(aInstance, aRegion, aFormat, ...)
#endif

// Logging at level note

#if GOBO_CONFIG_LOG_LEVEL >= GO_LOG_LEVEL_NOTE
#define goLogNote(aInstance, aRegion, aFormat, ...) \
    _goLogFormatter(aInstance, GO_LOG_LEVEL_NOTE, aRegion, _GO_LEVEL_NOTE_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogNote(aInstance, aRegion, aFormat, ...)
#endif

// Logging at level info

#if GOBO_CONFIG_LOG_LEVEL >= GO_LOG_LEVEL_INFO
#define goLogInfo(aInstance, aRegion, aFormat, ...) \
    _goLogFormatter(aInstance, GO_LOG_LEVEL_INFO, aRegion, _GO_LEVEL_INFO_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogInfo(aInstance, aRegion, aFormat, ...)
#endif

// Logging at level debug

#if GOBO_CONFIG_LOG_LEVEL >= GO_LOG_LEVEL_DEBG
#define goLogDebg(aInstance, aRegion, aFormat, ...) \
    _goLogFormatter(aInstance, GO_LOG_LEVEL_DEBG, aRegion, _GO_LEVEL_DEBG_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogDebg(aInstance, aRegion, aFormat, ...)
#endif

// Loging at region API

#if GOBO_CONFIG_LOG_API == 1
#define goLogCritApi(aInstance, aFormat, ...) \
    goLogCrit(&aInstance, GO_LOG_REGION_API, _GO_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#define goLogWarnApi(aInstance, aFormat, ...) \
    goLogWarn(&aInstance, GO_LOG_REGION_API, _GO_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#define goLogNoteApi(aInstance, aFormat, ...) \
    goLogNote(&aInstance, GO_LOG_REGION_API, _GO_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#define goLogInfoApi(aInstance, aFormat, ...) \
    goLogInfo(&aInstance, GO_LOG_REGION_API, _GO_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#define goLogDebgApi(aInstance, aFormat, ...) \
    goLogDebg(&aInstance, GO_LOG_REGION_API, _GO_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogCritApi(aInstance, aFormat, ...)
#define goLogWarnApi(aInstance, aFormat, ...)
#define goLogNoteApi(aInstance, aFormat, ...)
#define goLogInfoApi(aInstance, aFormat, ...)
#define goLogDebgApi(aInstance, aFormat, ...)
#endif

// Logging at region Memory

#if GOBO_CONFIG_LOG_MEM == 1
#define goLogCritMem(aInstance, aFormat, ...) \
    goLogCrit(&aInstance, GO_LOG_REGION_MEM, _GO_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#define goLogWarnMem(aInstance, aFormat, ...) \
    goLogWarn(&aInstance, GO_LOG_REGION_MEM, _GO_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#define goLogNoteMem(aInstance, aFormat, ...) \
    goLogNote(&aInstance, GO_LOG_REGION_MEM, _GO_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#define goLogInfoMem(aInstance, aFormat, ...) \
    goLogInfo(&aInstance, GO_LOG_REGION_MEM, _GO_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#define goLogDebgMem(aInstance, aFormat, ...) \
    goLogDebg(&aInstance, GO_LOG_REGION_MEM, _GO_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogCritMem(aInstance, aFormat, ...)
#define goLogWarnMem(aInstance, aFormat, ...)
#define goLogNoteMem(aInstance, aFormat, ...)
#define goLogInfoMem(aInstance, aFormat, ...)
#define goLogDebgMem(aInstance, aFormat, ...)
#endif

// Logging at region Platform

#if GOBO_CONFIG_LOG_PLATFORM == 1
#define goLogCritPlat(aInstance, aFormat, ...) \
    goLogCrit(aInstance, GO_LOG_REGION_PLATFORM, _GO_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#define goLogWarnPlat(aInstance, aFormat, ...) \
    goLogWarn(aInstance, GO_LOG_REGION_PLATFORM, _GO_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#define goLogNotePlat(aInstance, aFormat, ...) \
    goLogNote(aInstance, GO_LOG_REGION_PLATFORM, _GO_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#define goLogInfoPlat(aInstance, aFormat, ...) \
    goLogInfo(aInstance, GO_LOG_REGION_PLATFORM, _GO_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#define goLogDebgPlat(aInstance, aFormat, ...) \
    goLogDebg(aInstance, GO_LOG_REGION_PLATFORM, _GO_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogCritPlat(aInstance, aFormat, ...)
#define goLogWarnPlat(aInstance, aFormat, ...)
#define goLogInfoPlat(aInstance, aFormat, ...)
#define goLogDebgPlat(aInstance, aFormat, ...)
#endif

// Logging at region CLI

#if GOBO_CONFIG_LOG_CLI == 1

#define goLogCritCli(aInstance, aFormat, ...) \
    goLogCrit(&aInstance, GO_LOG_REGION_CLI, _GO_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#define goLogWarnCli(aInstance, aFormat, ...) \
    goLogWarn(&aInstance, GO_LOG_REGION_CLI, _GO_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#define goLogNoteCli(aInstance, aFormat, ...) \
    goLogNote(&aInstance, GO_LOG_REGION_CLI, _GO_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#define goLogInfoCli(aInstance, aFormat, ...) \
    goLogInfo(&aInstance, GO_LOG_REGION_CLI, _GO_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#define goLogInfoCliErr(aInstance, aError, aFormat, ...) \
    goLogInfo(&aInstance, GO_LOG_REGION_CLI, "Error %s: " aFormat, goErrorToString(aError), ##__VA_ARGS__)
#define goLogDebgCli(aInstance, aFormat, ...) \
    goLogDebg(&aInstance, GO_LOG_REGION_CLI, _GO_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogCritCli(aInstance, aFormat, ...)
#define goLogWarnCli(aInstance, aFormat, ...)
#define goLogNoteCli(aInstance, aFormat, ...)
#define goLogInfoCli(aInstance, aFormat, ...)
#define goLogInfoCliErr(aInstance, aError, aFormat, ...)
#define goLogDebgCli(aInstance, aFormat, ...)
#endif

// Logging at region Core

#if GOBO_CONFIG_LOG_CORE == 1
#define goLogCritCore(aInstance, aFormat, ...) \
    goLogCrit(&aInstance, GO_LOG_REGION_CORE, _GO_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define goLogWarnCore(aInstance, aFormat, ...) \
    goLogWarn(&aInstance, GO_LOG_REGION_CORE, _GO_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define goLogNoteCore(aInstance, aFormat, ...) \
    goLogNote(&aInstance, GO_LOG_REGION_CORE, _GO_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define goLogInfoCore(aInstance, aFormat, ...) \
    goLogInfo(&aInstance, GO_LOG_REGION_CORE, _GO_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define goLogDebgCore(aInstance, aFormat, ...) \
    goLogDebg(&aInstance, GO_LOG_REGION_CORE, _GO_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define goLogDebgCoreErr(aInstance, aError, aFormat, ...)                                                          \
    goLogWarn(aInstance, GO_LOG_REGION_CORE, _GO_REGION_CORE_PREFIX "Error %s: " aFormat, goErrorToString(aError), \
              ##__VA_ARGS__)
#else
#define goLogCritCore(aInstance, aFormat, ...)
#define goLogWarnCore(aInstance, aFormat, ...)
#define goLogInfoCore(aInstance, aFormat, ...)
#define goLogDebgCore(aInstance, aFormat, ...)
#define goLogDebgCoreErr(aInstance, aError, aFormat, ...)
#endif

// Logging at region Util

#if GOBO_CONFIG_LOG_UTIL == 1
#define goLogCritUtil(aInstance, aFormat, ...) \
    goLogCrit(&aInstance, GO_LOG_REGION_UTIL, _GO_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#define goLogWarnUtil(aInstance, aFormat, ...) \
    goLogWarn(&aInstance, GO_LOG_REGION_UTIL, _GO_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#define goLogNoteUtil(aInstance, aFormat, ...) \
    goLogNote(&aInstance, GO_LOG_REGION_UTIL, _GO_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#define goLogInfoUtil(aInstance, aFormat, ...) \
    goLogInfo(&aInstance, GO_LOG_REGION_UTIL, _GO_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#define goLogInfoUtilErr(aInstance, aError, aFormat, ...)                                                           \
    goLogInfo(&aInstance, GO_LOG_REGION_UTIL, _GO_REGION_CORE_PREFIX "Error %s: " aFormat, goErrorToString(aError), \
              ##__VA_ARGS__)
#define goLogDebgUtil(aInstance, aFormat, ...) \
    goLogDebg(&aInstance, GO_LOG_REGION_UTIL, _GO_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#else
#define goLogCritUtil(aInstance, aFormat, ...)
#define goLogWarnUtil(aInstance, aFormat, ...)
#define goLogNoteUtil(aInstance, aFormat, ...)
#define goLogInfoUtil(aInstance, aFormat, ...)
#define goLogInfoUtilErr(aInstance, aError, aFormat, ...)
#define goLogDebgUtil(aInstance, aFormat, ...)
#endif

void goDump(goInstance * aIntsance,
            goLogLevel   aLevel,
            goLogRegion  aRegion,
            const char * aId,
            const void * aBuf,
            const size_t aLength);

const char *goLogLevelToPrefixString(goLogLevel aLogLevel);

#define _goLogFormatter(aInstance, aLogLevel, aRegion, aFormat, ...) \
    _goDynamicLog(aInstance, aLogLevel, aRegion, aFormat GOBO_CONFIG_LOG_SUFFIX, ##__VA_ARGS__)

#if GOBO_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL == 1

#define _goDynamicLog(aInstance, aLogLevel, aRegion, aFormat, ...)  \
    do                                                              \
    {                                                               \
        if (goGetDynamicLogLevel(aInstance) >= aLogLevel)           \
            _goPlatLog(aLogLevel, aRegion, aFormat, ##__VA_ARGS__); \
    } while (false)

#else // GOBO_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL

#define _goDynamicLog(aInstance, aLogLevel, aRegion, aFormat, ...) \
    _goPlatLog(aLogLevel, aRegion, aFormat, ##__VA_ARGS__)

#endif // GOBO_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL

#define _goPlatLog(aLogLevel, aRegion, aFormat, ...) \
    GOBO_CONFIG_PLAT_LOG_FUNCTION(aLogLevel, aRegion, aFormat, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // LOGGING_HPP_
