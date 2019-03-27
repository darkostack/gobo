#include "cli_uart.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/wrap_string.h"

#include <gobo/cli.h>
#include <gobo/platform/logging.h>
#include <gobo/platform/uart.h>

#include "cli/cli.hpp"

#include "common/code_utils.hpp"
#include "common/encoding.hpp"
#include "common/logging.hpp"
#include "common/new.hpp"
#include "common/tasklet.hpp"

#if GOBO_CONFIG_ENABLE_DEBUG_UART
#include <gobo/platform/debug_uart.h>
#endif

namespace go {
namespace Cli {

static const char sCommandPrompt[] = {'>', ' '};
static const char sEraseString[]   = {'\b', ' ', '\b'};
static const char CRNL[]           = {'\r', '\n'};
Uart *            Uart::sUartServer;

static goDEFINE_ALIGNED_VAR(sCliUartRaw, sizeof(Uart), uint64_t);

extern "C" void goCliUartInit(goInstance *aInstance)
{
    Instance *instance = static_cast<Instance *>(aInstance);

    Uart::sUartServer = new (&sCliUartRaw) Uart(instance);
}

extern "C" void goCliUartSetUserCommands(const goCliCommand *aUserCommands, uint8_t aLength)
{
    Uart::sUartServer->GetInterpreter().SetUserCommands(aUserCommands, aLength);
}

extern "C" void goCliUartOutputBytes(const uint8_t *aBytes, uint8_t aLength)
{
    Uart::sUartServer->GetInterpreter().OutputBytes(aBytes, aLength);
}

extern "C" void goCliUartOutputFormat(const char *aFmt, ...)
{
    va_list aAp;
    va_start(aAp, aFmt);
    Uart::sUartServer->OutputFormatV(aFmt, aAp);
    va_end(aAp);
}

extern "C" void goCliUartAppendResult(goError aError)
{
    Uart::sUartServer->GetInterpreter().AppendResult(aError);
}

Uart::Uart(Instance *aInstance)
    : mInterpreter(aInstance)
{
    mRxLength   = 0;
    mTxHead     = 0;
    mTxLength   = 0;
    mSendLength = 0;

    goPlatUartEnable();
}

extern "C" void goPlatUartReceived(const uint8_t *aBuf, uint16_t aBufLength)
{
    Uart::sUartServer->ReceiveTask(aBuf, aBufLength);
}

void Uart::ReceiveTask(const uint8_t *aBuf, uint16_t aBufLength)
{
    const uint8_t *end;

    end = aBuf + aBufLength;

    for (; aBuf < end; aBuf++)
    {
        switch (*aBuf)
        {
        case '\r':
        case '\n':
            Output(CRNL, sizeof(CRNL));

            if (mRxLength > 0)
            {
                mRxBuffer[mRxLength] = '\0';
                ProcessCommand();
            }

            Output(sCommandPrompt, sizeof(sCommandPrompt));

            break;

        case '\b':
        case 127:
            if (mRxLength > 0)
            {
                Output(sEraseString, sizeof(sEraseString));
                mRxBuffer[--mRxLength] = '\0';
            }

            break;

        default:
            if (mRxLength < kRxBufferSize)
            {
                Output(reinterpret_cast<const char *>(aBuf), 1);
                mRxBuffer[mRxLength++] = static_cast<char>(*aBuf);
            }

            break;
        }
    }
}

goError Uart::ProcessCommand(void)
{
    goError error = GO_ERROR_NONE;

    if (mRxBuffer[mRxLength - 1] == '\n')
    {
        mRxBuffer[--mRxLength] = '\0';
    }

    if (mRxBuffer[mRxLength - 1] == '\r')
    {
        mRxBuffer[--mRxLength] = '\0';
    }

#if GOBO_CONFIG_LOG_OUTPUT != GOBO_CONFIG_LOG_OUTPUT_NONE
    goLogInfoCli(Instance::Get(), "execute command: %s", mRxBuffer);
#endif

    mInterpreter.ProcessLine(mRxBuffer, mRxLength, *this);

    mRxLength = 0;

    return error;
}

int Uart::Output(const char *aBuf, uint16_t aBufLength)
{
    uint16_t remaining = kTxBufferSize - mTxLength;
    uint16_t tail;

    if (aBufLength > remaining)
    {
        aBufLength = remaining;
    }

    for (int i = 0; i < aBufLength; i++)
    {
        tail            = (mTxHead + mTxLength) % kTxBufferSize;
        mTxBuffer[tail] = *aBuf++;
        mTxLength++;
    }

    Send();

    return aBufLength;
}

int Uart::OutputFormat(const char *fmt, ...)
{
    char    buf[kMaxLineLength];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    return Output(buf, static_cast<uint16_t>(strlen(buf)));
}

int Uart::OutputFormatV(const char *aFmt, va_list aAp)
{
    char buf[kMaxLineLength];

    vsnprintf(buf, sizeof(buf), aFmt, aAp);

    return Output(buf, static_cast<uint16_t>(strlen(buf)));
}

void Uart::Send(void)
{
    VerifyOrExit(mSendLength == 0);

    if (mTxLength > kTxBufferSize - mTxHead)
    {
        mSendLength = kTxBufferSize - mTxHead;
    }
    else
    {
        mSendLength = mTxLength;
    }

    if (mSendLength > 0)
    {
#if GOBO_CONFIG_ENABLE_DEBUG_UART
        /* duplicate the output to the debug uart */
        goPlatDebugUart_write_bytes(reinterpret_cast<uint8_t *>(mTxBuffer + mTxHead), mSendLength);
#endif
        goPlatUartSend(reinterpret_cast<uint8_t *>(mTxBuffer + mTxHead), mSendLength);
    }

exit:
    return;
}

extern "C" void goPlatUartSendDone(void)
{
    Uart::sUartServer->SendDoneTask();
}

void Uart::SendDoneTask(void)
{
    mTxHead = (mTxHead + mSendLength) % kTxBufferSize;
    mTxLength -= mSendLength;
    mSendLength = 0;

    Send();
}

extern "C" void goCliPlatLogv(goLogLevel aLogLevel, goLogRegion aLogRegion, const char *aFormat, va_list aArgs)
{
    VerifyOrExit(Uart::sUartServer != NULL);

    Uart::sUartServer->OutputFormatV(aFormat, aArgs);
    Uart::sUartServer->OutputFormat("\r\n");

    GO_UNUSED_VARIABLE(aLogLevel);
    GO_UNUSED_VARIABLE(aLogRegion);

exit:
    return;
}

} // namespace Cli
} // namespace go
