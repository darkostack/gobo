#include "logging.hpp"

#include "common/instance.hpp"

#if (GOBO_CONFIG_LOG_OUTPUT == GOBO_CONFIG_LOG_OUTPUT_DEBUG_UART) && (!GOBO_CONFIG_ENABLE_DEBUG_UART)
#error GOBO_CONFIG_ENABLE_DEBUG_UART_LOG requires GOBO_CONFIG_ENABLE_DEBUG_UART
#endif

#define goLogDump(aFormat, ...) \
    _goDynamicLog(aInstance, aLogLevel, aLogRegion, aFormat GOBO_CONFIG_LOG_SUFFIX, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

#if GOBO_CONFIG_LOG_PKT_DUMP == 1
void goDump(goInstance * aIntsance,
            goLogLevel   aLevel,
            goLogRegion  aRegion,
            const char * aId,
            const void * aBuf,
            const size_t aLength)
{
    size_t       idlen = strlen(aId);
    const size_t width = 72;
    char         buf[80];
    char *       cur = buf;

    for (size_t i = 0; i < (width - idlen) / 2 - 5; i++)
    {
        snprintf(cur, sizeof(buf) - static_cast<size_t>(cur - buf), "=");
        cur += strlen(cur);
    }

    snprintf(cur, sizeof(buf) - static_cast<size_t>(cur - buf), "[%s len=%03u]", aId, static_cast<uint16_t>(aLength));
    cur += strlen(cur);

    for (size_t i = 0; i < (width - idlen) / 2 - 4; i++)
    {
        snprintf(cur, sizeof(buf) - static_cast<size_t>(cur - buf), "=");
        cur += strlen(cur);
    }

    goLogDump("%s", buf);

    for (size_t i = 0; i < aLength; i += 16)
    {
        DumpLine(aInstance, aLogLevel, aLogRegion, (uint8_t *)(aBuf) + i, (aLength - i) < 16 ? (aLength - i) : 16);
    }

    cur = buf;

    for (size_t i = 0; i < width; i++)
    {
        snprintf(cur, sizeof(buf) - static_cast<size_t>(cur - buf), "-");
        cur += strlen(cur);
    }

    goLogDump("%s", buf);
}
#else
void goDump(goInstance *, goLogLevel, goLogRegion, const char *, const void *, const size_t)
{
}
#endif // GOBO_CONFIG_LOG_PKT_DUMP == 1

const char *goErrorToString(goError aError)
{
    const char *retval;

    switch (aError)
    {
    case GO_ERROR_NONE:
        retval = "OK";
        break;

    case GO_ERROR_FAILED:
        retval = "Failed";
        break;

    case GO_ERROR_DROP:
        retval = "Drop";
        break;

    case GO_ERROR_NO_BUFS:
        retval = "NoBufs";
        break;

    case GO_ERROR_BUSY:
        retval = "Busy";
        break;

    case GO_ERROR_PARSE:
        retval = "Parse";
        break;

    case GO_ERROR_INVALID_ARGS:
        retval = "InvalidArgs";
        break;

    case GO_ERROR_SECURITY:
        retval = "Security";
        break;

    case GO_ERROR_ABORT:
        retval = "Abort";
        break;

    case GO_ERROR_NOT_IMPLEMENTED:
        retval = "NotImplemented";
        break;

    case GO_ERROR_INVALID_STATE:
        retval = "InvalidState";
        break;

    case GO_ERROR_NOT_FOUND:
        retval = "NotFound";
        break;

    case GO_ERROR_ALREADY:
        retval = "Already";
        break;

    case GO_ERROR_NOT_CAPABLE:
        retval = "NotCapable";
        break;

    case GO_ERROR_RESPONSE_TIMEOUT:
        retval = "ResponseTimeout";
        break;

    case GO_ERROR_DISABLED_FEATURE:
        retval = "DisabledFeature";
        break;

    case GO_ERROR_GENERIC:
        retval = "GenericError";
        break;

    default:
        retval = "UnknownErrorType";
        break;
    }

    return retval;
}

const char *goLogLevelToPrefixString(goLogLevel aLogLevel)
{
    const char *retval = "";

    switch (aLogLevel)
    {
    case GO_LOG_LEVEL_NONE:
        retval = _GO_LEVEL_NONE_PREFIX;
        break;

    case GO_LOG_LEVEL_CRIT:
        retval = _GO_LEVEL_CRIT_PREFIX;
        break;

    case GO_LOG_LEVEL_WARN:
        retval = _GO_LEVEL_WARN_PREFIX;
        break;

    case GO_LOG_LEVEL_NOTE:
        retval = _GO_LEVEL_NOTE_PREFIX;
        break;

    case GO_LOG_LEVEL_INFO:
        retval = _GO_LEVEL_INFO_PREFIX;
        break;

    case GO_LOG_LEVEL_DEBG:
        retval = _GO_LEVEL_DEBG_PREFIX;
        break;
    }

    return retval;
}

#if GOBO_CONFIG_LOG_OUTPUT == GOBO_CONFIG_LOG_OUTPUT_NONE
/* this provides a stub, incase something uses the function */
void goPlatLog(goLogLevel aLogLevel, goLogRegion aLogRegion, const char *aFormat, ...)
{
    GO_UNUSED_VARIABLE(aLogLevel);
    GO_UNUSED_VARIABLE(aLogRegion);
    GO_UNUSED_VARIABLE(aFormat);
}
#endif

#ifdef __cplusplus
}
#endif
