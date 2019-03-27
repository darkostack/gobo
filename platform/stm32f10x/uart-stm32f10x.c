#include "platform-stm32f10x.h"

#include <gobo/platform/debug_uart.h>
#include <gobo/platform/uart.h>

#include "utils/code_utils.h"

#define QUEUE_ELEMENTS 128
#define QUEUE_SIZE (QUEUE_ELEMENTS + 1)

static uint8_t sQueue[QUEUE_SIZE];
int sQueueIn, sQueueOut;

static const uint8_t *sWriteBuffer;
static uint16_t sWriteLength;
static uint16_t sWriteIndex;

static void queueInit(void)
{
    sQueueIn = 0;
    sQueueOut = 0;
}

static int queuePut(uint8_t aByte)
{
    if (sQueueIn == ((sQueueOut - 1 + QUEUE_SIZE) % QUEUE_SIZE)) {
        return -1; /* queue full */
    }
    sQueue[sQueueIn] = aByte;
    sQueueIn = (sQueueIn + 1) % QUEUE_SIZE;
    return 0;
}

static int queueGet(uint8_t *aByte)
{
    if (sQueueIn == sQueueOut) {
        return -1; /* queue empty - nothing to get */
    }
    *aByte = sQueue[sQueueOut];
    sQueueOut = (sQueueOut + 1) % QUEUE_SIZE;
    return 0;
}

static bool queueIsEmpty(void)
{
    return (sQueueIn == sQueueOut) ? true : false;
}

goError goPlatUartEnable(void)
{
    sWriteLength = sWriteIndex = 0;
    sWriteBuffer = NULL;

    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // Configure USART Tx (PA9) as alternal function push-pull
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure USART Rx (PA10) as input floating
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Enable USART1 interrupt
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_Init(USART1, &USART_InitStruct);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

    queueInit();

    return GO_ERROR_NONE;
}

goError goPlatUartDisable(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_DeInit(USART1);
    USART_Cmd(USART1, DISABLE);

    return GO_ERROR_NONE;
}

goError goPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
    goError error = GO_ERROR_NONE;

    goEXPECT_ACTION(sWriteLength == 0, error = GO_ERROR_BUSY);

    sWriteBuffer = aBuf;
    sWriteLength = aBufLength;

exit:
    return error;
}

void platformUartProcess(void)
{
    uint8_t receiveByte;

    if (!queueIsEmpty())
    {
        queueGet(&receiveByte);

        goPlatUartReceived(&receiveByte, 1);
    }

    if ((sWriteLength != sWriteIndex) && (sWriteBuffer != NULL))
    {
        USART_SendData(USART1, (uint8_t)sWriteBuffer[sWriteIndex++]);

        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
            // Loop until the end of transmission
        }

        if (sWriteLength == sWriteIndex)
        {
            sWriteLength = sWriteIndex = 0;
            goPlatUartSendDone();
        }
    }
}

void platformUartRestore(void)
{
    sWriteLength = sWriteIndex = 0;
    sWriteBuffer = NULL;

    queueInit();
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        queuePut((uint8_t)USART_ReceiveData(USART1));
    }
}
