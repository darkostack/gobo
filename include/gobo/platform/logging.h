#ifndef GO_PLATFORM_LOGGING_H_
#define GO_PLATFORM_LOGGING_H_

#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GO_LOG_LEVEL_NONE 0
#define GO_LOG_LEVEL_CRIT 1
#define GO_LOG_LEVEL_WARN 2
#define GO_LOG_LEVEL_NOTE 3
#define GO_LOG_LEVEL_INFO 4
#define GO_LOG_LEVEL_DEBG 5

typedef uint8_t goLogLevel;

typedef enum goLogRegion {
    GO_LOG_REGION_API      = 1,
    GO_LOG_REGION_MEM      = 2,
    GO_LOG_REGION_PLATFORM = 3,
    GO_LOG_REGION_CLI      = 4,
    GO_LOG_REGION_CORE     = 5,
    GO_LOG_REGION_UTIL     = 6,
} goLogRegion;

void goPlatLog(goLogLevel aLogLevel, goLogRegion aLogRegion, const char *aFormat, ...);

void goPlatLogv(goLogLevel aLogLevel, goLogRegion aLogRegion, const char *aFormat, va_list ap);

#ifdef __cplusplus
}
#endif

#endif // GO_PLATFORM_LOGGING_H_
