#include "platform-stm32f10x.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <gobo/platform/logging.h>
#include <gobo/platform/toolchain.h>

#include "utils/code_utils.h"

void platformLoggingInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // Configure USART Tx (PA2) as alternate function push-pull
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure USART Rx (PA3) as input floating
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_Init(USART2, &USART_InitStruct);
    USART_Cmd(USART2, ENABLE);
}

static void platLogOut(char *aBuf, uint32_t aLength)
{
    for (uint32_t i = 0; i < aLength; i++)
    {
        USART_SendData(USART2, (uint8_t)aBuf[i]);

        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        {
            // Loop until the end of transmission
        }
    }
}

#if (GOBO_CONFIG_LOG_OUTPUT == GOBO_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED)
GO_TOOL_WEAK void goPlatLog(goLogLevel aLogLevel, goLogRegion aLogRegion, const char *aFormat, ...)
{
    char         logString[512];
    unsigned int offset;
    int          charsWritten;
    va_list      args;
    char CRNL[] = {'\r', '\n'};

    offset = 0;

    va_start(args, aFormat);
    charsWritten = vsnprintf(&logString[offset], sizeof(logString) - offset, aFormat, args);
    va_end(args);

    goEXPECT_ACTION(charsWritten >= 0, logString[offset] = 0);

exit:
    platLogOut(logString, charsWritten);
    platLogOut(CRNL, sizeof(CRNL));

    GO_UNUSED_VARIABLE(aLogLevel);
    GO_UNUSED_VARIABLE(aLogRegion);
}
#endif // #if (GOBO_CONFIG_LOG_OUTPUT == GOBO_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED)
