#ifndef GOBO_CLI_H_
#define GOBO_CLI_H_

#include <stdarg.h>
#include <stdint.h>

#include <gobo/instance.h>
#include <gobo/platform/logging.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct goCliCommand
{
    const char *mName;
    void (*mCommand)(int aArgc, char *aArgv[]);
} goCliCommand;

typedef int (*goCliCommandOuputCallback)(const char *aBuf, uint16_t aBufLength, void *aContext);

void goCliUartInit(goInstance *aInstance);

void goCliUartSetUserCommands(const goCliCommand *aUserCommands, uint8_t aLength);

void goCliUartOutputBytes(const uint8_t *aBytes, uint8_t aLength);

void goCliUartOutputFormat(const char *aFmt, ...);

void goCliUartAppendResult(goError aError);

void goCliPlatLogv(goLogLevel aLogLevel, goLogRegion aLogRegion, const char *aFormat, va_list aArgs);

#ifdef __cplusplus
}
#endif

#endif // GOBO_CLI_H_
