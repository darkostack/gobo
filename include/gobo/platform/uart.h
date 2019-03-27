#ifndef GOBO_PLATFORM_UART_H_
#define GOBO_PLATFORM_UART_H_

#include <stdint.h>

#include <gobo/error.h>

#ifdef __cplusplus
extern "C" {
#endif

goError goPlatUartEnable(void);

goError goPlatUartDisable(void);

goError goPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength);

extern void goPlatUartSendDone(void);

extern void goPlatUartReceived(const uint8_t *aBuf, uint16_t aBufLength);

#ifdef __cplusplus
}
#endif

#endif // NESX_PLATFORM_UART_H_
