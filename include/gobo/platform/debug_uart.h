#ifndef GOBO_PLATFORM_DEBUG_UART_H_
#define GOBO_PLATFORM_DEBUG_UART_H_

#include <gobo/error.h>
#include <gobo/platform/logging.h>

#if __cplusplus
extern "C" {
#endif

void goPlatDebugUart_printf(const char *aFormat, ...);

void goPlatDebugUart_vprintf(const char *aFormat, va_list ap);

void goPlatDebugUart_putchar_raw(int aChar);

int goPlatDebugUart_kbhit(void);

int goPlatDebugUart_getc(void);

void goPlatDebugUart_putchar(int aChar);

void goPlatDebugUart_puts(const char *aString);

void goPlatDebugUart_write_bytes(const uint8_t *aBytes, int aNumBytes);

void goPlatDebugUart_puts_no_nl(const char *aString);

goError goPlatDebugUart_logfile(const char *aFilename);

#if __cplusplus
}
#endif

#endif // GOBO_PLATFORM_DEBUG_UART_H_
