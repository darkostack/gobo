#include <gobo-core-config.h>

#include <stdarg.h>
#include <stdio.h>

#include <gobo/platform/alarm-milli.h>
#include <gobo/platform/debug_uart.h>
#include <gobo/platform/toolchain.h>

GO_TOOL_WEAK
void goPlatDebugUart_printf(const char *aFormat, ...)
{
    va_list ap;
    va_start(ap, aFormat);
    goPlatDebugUart_vprintf(aFormat, ap);
    va_end(ap);
}

GO_TOOL_WEAK
void goPlatDebugUart_vprintf(const char *aFormat, va_list ap)
{
    char buf[128];
    vsnprintf(buf, sizeof(buf), aFormat, ap);
    buf[sizeof(buf) - 1] = 0;
    goPlatDebugUart_puts_no_nl(buf);
}

GO_TOOL_WEAK
void goPlatDebugUart_write_bytes(const uint8_t *aBytes, int aNumBytes)
{
    while (aNumBytes > 0)
    {
        goPlatDebugUart_putchar((int)(*aBytes));
        aBytes++;
        aNumBytes--;
    }
}

GO_TOOL_WEAK
void goPlatDebugUart_puts_no_nl(const char *aString)
{
    while (*aString)
    {
        goPlatDebugUart_putchar(*aString);
        aString++;
    }
}

GO_TOOL_WEAK
void goPlatDebugUart_puts(const char *aString)
{
    goPlatDebugUart_puts_no_nl(aString);
    goPlatDebugUart_putchar('\n');
}

GO_TOOL_WEAK
void goPlatDebugUart_putchar(int aChar)
{
    if (aChar == '\n')
    {
        goPlatDebugUart_putchar_raw('\r');
    }

    goPlatDebugUart_putchar_raw(aChar);
}

// provide WEAK stubs for platform that do not implement all these functions.

GO_TOOL_WEAK
void goPlatDebugUart_putchar_raw(int aChar)
{
    GO_UNUSED_VARIABLE(aChar);
}

GO_TOOL_WEAK
int goPlatDebugUart_kbhit(void)
{
    return 0;
}

GO_TOOL_WEAK
int goPlatDebugUart_getc(void)
{
    return -1;
}

GO_TOOL_WEAK
goError goPlatDebugUart_logfile(const char *aFilename)
{
    GO_UNUSED_VARIABLE(aFilename);
    return GO_ERROR_FAILED;
}

#if (GOBO_CONFIG_LOG_OUTPUT == GOBO_CONFIG_LOG_OUTPUT_DEBUG_UART)
void goPlatLog(goLogLevel aLogLevel, goLogRegion aLogRegion, const char *aFormat, ...)
{
    va_list ap;
    uint32_t now;

    GO_UNUSED_VARIABLE(aLogLevel);
    GO_UNUSED_VARIABLE(aLogRegion);

    now = goPlatAlarmMilliGetNow();
    goPlatDebugUart_printf("%3d.%03d | ", (int)(now / 1000), (int)(now % 1000));
    va_start(ap, aFormat);
    goPlatDebugUart_vprintf(aFormat, ap);
    va_end(ap);

    goPlatDebugUart_putchar('\n');
}
#endif
